import java.io.*;
import java.net.*;

public class c5p2a {

	private ServerSocket ssocket; // do nasluchu (for listening)
	private int port; // nr portu (port number)

	public c5p2a(int aport) throws IOException {
		port = aport;

		// utworzenie gniazda nasluchu
		// i powiazanie go z portem (bind)
		// creation of "listen" socket and binding with port
		ssocket = new ServerSocket(port);
	}

	public void startListening() {
		Socket socket = null; // dla jednego klienta (for one client)
		int ivalue;
		String svalue;
		double dvalue;

		try {
			// czekaj na polaczenie (wait for connection)
			socket = ssocket.accept();

			// pobierz strumienie i nadbuduj
			// na nich "lepsze" strumienie
			// (receive straems and build "better" streams)
			DataInputStream dis = new DataInputStream(
					socket.getInputStream());
			DataOutputStream dos = new DataOutputStream(
					socket.getOutputStream());

			// czytaj kolejno int, String i double (read: int, string, double)
			ivalue = dis.readInt();
			svalue = dis.readUTF();
			dvalue = dis.readDouble();

			// wypisz co odebrano (print what was received)
			System.out.println("Odebrano (received):");
			System.out.println(ivalue);
			System.out.println(svalue);
			System.out.println(dvalue);

			// odpisz klientowi
			dos.writeUTF("ODEBRANO POPRAWNIE (received correctly)");

			// zamknij strumienie (close streams)
			dis.close();
			dos.close();

			// zamknij gniazdo (close sockets)
			socket.close();

			// zamknij gniazdo nasluchujace (close listening socket)
			ssocket.close();

			// jesli cos pojdzie zle, wypisz
			// stos wywolan
			// if anything goes wrong, print stack trace
		} catch (Exception e) {
			e.printStackTrace();
			return;
		}
	}

	// args[0] - numer portu (port number)
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println(
					"Podaj numer portu (provide port number)"
					);
			return;
		}
		try {
			// ustal port
			int port = Integer.parseInt(args[0]);
			c5p2a server = new c5p2a(port);
			System.out.println("Czekam (I am waiting)...");
			server.startListening();

			// moze byc albo wyjatek z konstruktora
			// c5p2a albo z Integer.parseInt jesli parametr
			// nie jest liczba
			// possible exception if paremeter Integer.parseInt is not a number
		} catch (Exception e) {
			e.printStackTrace();
		}
		System.out.println("Serwer zakonczyl dzialanie (server finished).");
	}
}
