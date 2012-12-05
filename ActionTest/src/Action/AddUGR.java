package Action;

import java.util.ArrayList;

import elements.AddUser;
import elements.PostPhoto;

public class AddUGR {
	private static int usernum = 0;
	private static int photonum = 0;
	private static String address;
	public static void main(String args[]) {
		if (args.length != 6) {
			System.out
					.println("Usage: -user XX -photo XX -address XX");
			return;
		}
		for (int i = 0; i < args.length; ++i) {
			if (args[i].equals("-user"))
				usernum = Integer.parseInt(args[++i]);
			else if (args[i].equals("-photo"))
				photonum = Integer.parseInt(args[++i]);
			else if (args[i].equals("-address"))
				address = args[++i];
		}
		adduser();
		System.out.println("add user:" + usernum);
		addphoto();
		System.out.println("add photo:" + photonum);
	}

	public static void adduser() {
		AddUser add = new AddUser(0, usernum, address);
		add.add();
	}

	public static void addphoto() {
		ArrayList<String> files = new ArrayList<String>();
		files.add("test.png");
		PostPhoto post = new PostPhoto(0, photonum, address, usernum,files);
		post.add();
	}

}
