#include <iostream>
#include <string>
#include <glog/logging.h>
#include <Magick++.h>
#include "base64.h"
#include "hyperclient.h"

extern "C" const std::string
handler(const char *key,
        size_t key_sz,
        hyperclient_attribute *attrs,
        size_t attrs_sz,
        hyperclient *message)
{
    //check the validation of attrs
    size_t index_content = -1;
    size_t index_username = -1;
    size_t index_time = -1;
    for(size_t i=0; i<attrs_sz; i++)
    {
        LOG(INFO)<<"attr "<<attrs[i].attr<<", value : "<<std::string(attrs[i].value, attrs[i].value_sz);
        if(!strcmp(attrs[i].attr, "content"))
            index_content = i;

    }
    if(index_content == -1) 
    {
        LOG(INFO)<<"photo content not found";
    }

    //decode the photo content
    std::string decoded_img;
    decoded_img = base64_decode(std::string(attrs[index_content].value, attrs[index_content].value_sz));

    //load to Magick blob
    Magick::Blob blob(decoded_img.c_str(), decoded_img.size());

    //chop the image x_off = 100, y_off = 100, x = 100, y = 100
    Magick::Image image(blob);
    image.crop(Magick::Geometry(100, 100, 100, 100));

    //put this image into image_s space
    Magick::Blob out_blob;
    image.magick("JPEG");
    image.write(&out_blob);

    std::string encoded_img = base64_encode(reinterpret_cast<const unsigned char *>(out_blob.data()), out_blob.length());

    hyperclient_attribute attrs_put[1];
    attrs_put[0].attr = "content";
    attrs_put[0].value = encoded_img.c_str();
    attrs_put[0].value_sz = strlen(attrs_put[0].value);
    attrs_put[0].datatype = HYPERDATATYPE_STRING;

    //call put
    hyperclient_returncode retcode;
    int64_t ret = message->put("photo_s", key, key_sz, attrs_put, 1, &retcode);

    //loop till put finish
    hyperclient_returncode loop_status;
    int64_t loop_id = message->loop(-1, &loop_status);

    if(ret != loop_id)
    {
        LOG(INFO)<<"loop on put failed in trigger handler";
    }

    LOG(INFO)<<"put success and the return code is "<<retcode;

    return "";
}
