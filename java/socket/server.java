import java.io.BufferededReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class server {
    public static void main(String[] args) {
	final int port = 1345;
	ServerSocket serv = new ServerSocket(port);
	
	while(true) {
	    Socket sock = serv.accept();	
	
	    OutputStream os = sock.getOutputStream();
	    PrintWriter pw = new PrintWriter(os, true);
	    pw.println("what's your name ?");
	    System.out.println("server says: what's your name ?");
	
	    BufferedReader br = new BufferedReader(new InputStreamReader(serv.getInputStream() ) );
	    String name = br.readline();

	    pw.println("hello," + name);
	    System.out.println("server says: hello, " + name);
	    pw.close();
	    //sock.close();
	}
    }
}	
