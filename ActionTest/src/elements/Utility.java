package elements;


import java.io.IOException;
import java.io.OutputStream;
import java.net.Socket;


public class Utility {
	private Socket socket = null;

	public Utility(Socket socket) {
		this.socket = socket;
	}

	public void SendRequest(String request, String url, String version)
			throws IOException {
		OutputStream os = socket.getOutputStream();
		os.write((request + " " + url + " " + version + " " + Constants.CRLF)
				.getBytes());
		os.flush();
	}

	public void SendTag(String tag, String content) throws IOException {
		OutputStream os = socket.getOutputStream();
		os.write((tag + content + Constants.CRLF).getBytes());
		os.flush();
	}

	public void SendEnd() throws IOException {
		OutputStream os = socket.getOutputStream();
		os.write(Constants.CRLF.getBytes());
		os.flush();
	}
}
