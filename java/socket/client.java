import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class client {
    public static void main(string args[]) {
	final int port = 1345;
	final hostname = "localhost";
	clt = new Socket(hostname, port);
	
	while(true) {
	    BufferReader br = new BufferReader(new InputStreamReader(clt.getInputStream()));
	    System.out.println("Server ask: " + br.readline());
	
	    BufferRead userInput = new BUfferReader(new InputStreamReader(System.in));
	    String name = userInput.readline();
	    System.out.println(name);
	
	    PrintWriter pw = new PrintWriter(clt.getOutputStream(), true);
	    pw.println(name);

	    if("exit".equalsIgnoreCase(userInput)) {
		clt.close();
		break;
	    }
	}
    }
}
