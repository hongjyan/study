import java.util.regex.Pattern;
import java.util.regex.Matcher;

public class regexTest {
    public static void main(String[] args) {
	String s = "Start with a good word and end with a kind deed";
	String s2 = "Start with a kind word and end with a good deed";
	String regex = "^Start (?=.*kind) (?=.*good) deed$";
	System.out.println(s.matches(regex));
	System.out.println(s2.matches(regex));
    
	String channelName = "{channel_T1, channel_T100}";
	Pattern p = Pattern.compile("^\\{([_0-9A-Za-z]*[_A-Za-z]+)([0-9]+),\\s*\\1([0-9]+)\\}");
	Matcher m = p.matcher(channelName);
	if(!m.find()) {
	    System.out.println("not match");
	}
	else {
	    System.out.println(m.group(1));
	    System.out.println(m.group(2));
	    System.out.println(m.group(3));
	}
    }
}
