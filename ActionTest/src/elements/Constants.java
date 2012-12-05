package elements;


public class Constants {
	// query params
	public final static String Comp = "comp";
	public final static String BlockId = "blockid";

	// comp type
	public final static String Metadata = "metadata";
	public final static String Block = "block";

	// http constants
	// response type
	public static final String BadRequest = "400 Bad Request";
	public static final String NotFound = "404 Not Found";
	public static final String NotModified = "304 Not Modified";
	public static final String OK = "200 OK";
	public static final String ServerError = "500 Internal Server Error";
	// header
	public static final String Connection = "Connection:";
	public static final String Content_Type = "Content-Type:";
	public static final String Content_Length = "Content-Length:";
	public static final String If_Modified_Since = "If-Modified-Since:";
	public static final String Last_Modified = "Last-Modified:";
	public static final String Range = "Range:";
	public static final String CRLF = "\r\n";
	// method
	public static final String GET = "GET";
	public static final String PUT = "PUT";

	// constants
	public static int _1KB = 1024;
	public static int _4MB = 4 * 1024 * 1024;
}
