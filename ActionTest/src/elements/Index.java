package elements;
import java.util.Random;

public class Index {
	private int num;
	private String ip;
	private int maxuserid;	private long start;

	public Index(int maxuserid,int num,String ip) {
		this.maxuserid=maxuserid;
		this.num = num;
		this.ip=ip;
	}

	public long add() {
		//try {
		start=System.currentTimeMillis();

		Random ran = new Random();
			for (int i = 0; i < this.num; ++i) {
				System.out.println(HttpTookit.doGet("http://"+ip+"/index.php?username="+ran.nextInt(maxuserid)));
				/*Socket socket = new Socket(ip, port);
				Utility util = new Utility(socket);
				
				
				util.SendRequest("GET",
						"/buyrecord.php?id=" +ran.nextInt(maxuserid), "HTTP/1.1");
				util.SendTag("Host: ", "localhost:8080");
				util.SendTag("User-agent: ", "OnePieceClient/0.1");
				util.SendTag("Connection: ", "keep-alive");
				util.SendTag("Accept: ", "text/plain");
				util.SendEnd();
				DataInputStream in = new DataInputStream(
						socket.getInputStream());
				String str = null;
				while ((str = in.readLine()) != null) {
					System.out.println(str);
				}*/
			}
		/*} catch (UnknownHostException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}*/return (System.currentTimeMillis()-start)/num;
	}

	public static void main(String args[]) {
		Index add = new Index(100,1,"localhost");
		System.out.println(add.add());
	}
}
