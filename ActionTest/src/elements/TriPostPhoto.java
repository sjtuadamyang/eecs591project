package elements;

import java.io.ByteArrayOutputStream;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Random;

import org.apache.commons.codec.binary.Base64;

public class TriPostPhoto {
	private int id;
	private int num;
	private String ip;
	private ArrayList<String> filenames;
	private int maxuserid;
	private long start;
	private ArrayList<String> contents = new ArrayList<String>();

	public TriPostPhoto(int id, int num, String ip, int maxuserid,
			ArrayList<String> filenames) {
		this.id = id;
		this.num = num;
		this.ip = ip;
		this.maxuserid = maxuserid;
		this.filenames = filenames;
		// read contents of files
		try {
			Iterator<String> iter = filenames.iterator();
			while (iter.hasNext()) {
				InputStream is = new FileInputStream(iter.next());
				byte[] buf=new byte[4096];
				ByteArrayOutputStream baos = new ByteArrayOutputStream();
				while(true) {
					  int n = is.read(buf);
					  if( n < 0 ) break;
					  baos.write(buf,0,n);
				}
				byte[] result=baos.toByteArray();
				String encode=Base64.encodeBase64String(result);
				contents.add(encode);
				System.out.println(encode);
			}
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public long add() {
		// try {
		start = System.currentTimeMillis();

		for (int i = 0; i < this.num; ++i) {

			Random ran = new Random();
			int id = this.id + i;
			System.out.println(id);
			int userid = ran.nextInt(maxuserid);
			
			HashMap<String, String> params = new HashMap<String, String>();
			params.put("username", String.valueOf(userid));
			params.put("imageraw",contents.get(ran.nextInt(contents.size())));
			HttpTookit.doPost("http://" + ip + "/upload_triput.php", params);
		}

		return (System.currentTimeMillis() - start) / num;
	}

	public static void main(String args[]) {
		ArrayList<String> files = new ArrayList<String>();
		files.add("test.png");
		TriPostPhoto add = new TriPostPhoto(0, 1, "localhost", 1, files);
		add.add();
	}

	public ArrayList<String> getFilenames() {
		return filenames;
	}

	public void setFilenames(ArrayList<String> filenames) {
		this.filenames = filenames;
	}
	
}
