package Action;

import java.util.ArrayList;

import elements.PostPhoto;

public class ClientSimulatorPut {
	private String ip;
	private int maxuserid;
	private int threadnum;
	private int num;
	private ArrayList<String> files;

	public ClientSimulatorPut(String ip, int maxuserid, int threadnum, int num) {
		this.ip = ip;
		this.maxuserid = maxuserid;
		this.threadnum = threadnum;
		this.num = num;
		files = new ArrayList<String>();
		files.add("test.png");
	}

	public static void main(String args[]) {
		if (args.length != 3)
			System.out.println("usage: ip maxuserid threadnum num");
		String ip = args[0];
		int maxuserid = Integer.valueOf(args[1]);
		int threadnum = Integer.valueOf(args[2]);
		int num = Integer.valueOf(args[3]);
		ClientSimulatorPut client = new ClientSimulatorPut(ip, maxuserid, threadnum,
				num);
		client.run();

	}

	public void run() {
		System.out.println("Thread num:" + threadnum);
		for (int i = 0; i < threadnum; ++i) {
			Runnable r = new Runnable() {
				public void run() {
					PostPhoto post = new PostPhoto(0, num, ip, maxuserid, files);
					long result = post.add();
					System.out.println("thread ends:  num:" + num
							+ "  average:" + result);
				}
			};
			Thread t = new Thread(r);
			t.start();
		}
	}
}
