function html_decode(str) {
	var s = "";
	if (str.length == 0)
		return "";
	var tmp = "";
	s = str;
	do {
		tmp = s;

		s = s.replace("&ensp;", " ");
		s = s.replace("&nbsp;", " ");
		s = s.replace("&emsp;", "　");
		s = s.replace("&reg;", "®");
		s = s.replace("&lt;", "<");
		s = s.replace("&gt;", ">");
		s = s.replace("&ldquo;", "\"");
		s = s.replace("&rdquo;", "\"");
		s = s.replace("&quot;", "\"");
		s = s.replace("&lsquo;", "‘");
		s = s.replace("&rsquo;", "’");
		s = s.replace("&mdash;", "—");
		s = s.replace("&ndash;", "–");
		s = s.replace("&middot;", "·");
		s = s.replace("&trade;", "™");
		s = s.replace("&copy;", "©");
		s = s.replace("&hellip;", "…");
		s = s.replace("<br>", "\r\n");
		s = s.replace("<br/>", "\r\n");
		s = s.replace("<br/>", "\r\n");
		s = s.replace("  ", "　");
		s = s.replace("&amp;", "&");

	} while (tmp != s)

	return s;
}