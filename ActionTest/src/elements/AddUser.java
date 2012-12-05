package elements;

import java.util.HashMap;
import java.util.Random;

public class AddUser {
	private int id;
	private int num;
	private String ip;
	private long start;

	public AddUser(int id, int num, String ip) {
		this.id = id;
		this.num = num;
		this.ip = ip;
	}

	public long add() {
		/* try { */
		start = System.currentTimeMillis();
		for (int i = 0; i < this.num; ++i) {
			/*
			 * Socket socket = new Socket(ip, port); Utility util = new
			 * Utility(socket);
			 */
			Random ran = new Random();
			int id = this.id + i;
			System.out.println(id);
			String pass = String.valueOf(ran.nextInt());
			String email = String.valueOf(ran.nextInt());

			HashMap<String, String> params = new HashMap<String, String>();
			params.put("username", String.valueOf(id));
			params.put("password",pass);
			params.put("info", email);
			HttpTookit.doGet("http://" + ip + "/591demo/register.php?username="+params.get("username")+"&password="+pass);
			
			/*
			 * util.SendRequest("GET", "/register.php?username=" +
			 * String.valueOf(id) + "&password=" + pass + "&gender=" + gender +
			 * "&birth=" + birth, "HTTP/1.1"); util.SendTag("Host: ",
			 * "localhost:8080"); util.SendTag("User-agent: ",
			 * "OnePieceClient/0.1"); util.SendTag("Connection: ",
			 * "keep-alive"); util.SendTag("Accept: ", "text/plain");
			 * util.SendEnd(); DataInputStream in = new DataInputStream(
			 * socket.getInputStream()); String str = null; while ((str =
			 * in.readLine()) != null) { System.out.println(str); }
			 */
		}
		/*
		 * } catch (UnknownHostException e) { e.printStackTrace(); } catch
		 * (IOException e) { e.printStackTrace(); }
		 */
		return (System.currentTimeMillis() - start) / num;
	}

	public static void main(String args[]) {
		AddUser add = new AddUser(0, 10, "localhost");
		add.add();
	}
}
