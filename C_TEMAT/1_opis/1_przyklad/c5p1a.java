import java.io.*;
import java.net.*;

public class c5p1a {

	private DatagramSocket socket; // gniazdo UDP

	public c5p1a(int aport) throws IOException {
		// utworzenie i powiazanie z portem
		// creation and binding on port
		socket = new DatagramSocket(aport);
	}

	void dataExchange() {
		byte[] bufor = new byte[256];
		// "pusty" pakiet do odbioru danych
		// "empty" packet for receiving data
		DatagramPacket p = new DatagramPacket(bufor, 256);
		try {
			socket.receive(p); // czekaj na datagram (wait for datagram)

			// napisz kto jest nadawca (print who is sender)
			System.out.println(
					"Od: "+p.getAddress().toString()+
					" ("+p.getAddress().getHostName()+")");

			// utworz lancuch z tablicy bajtow (create string from array of bytes)
			String s = new String(p.getData());

			// wypisz wiadomosc (print message)
			System.out.println(s);

			// utworz datagram zwrotny (create reply datagram)
			// korzystajac z adresu nadawcy (using sender address)
			String response = "Odebrano (Received).";
			DatagramPacket p2 = new DatagramPacket(
					response.getBytes(), response.length(),
					p.getAddress(), p.getPort());

			socket.send(p2); // wyslij odpowiedz (send reply)
			System.out.println("Odsyłam uzyskane dane");
			socket.close(); // koniec protokolu (end of protocol)

			// jesli cos poszlo nie tak (if anything went wrong)
			// wypisz stan stosu wywolan (print stack trace)
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// args[0] - numer portu w wierszu polecen (port number in command line)
	public static void main(String[] args) {
		if (args.length < 1) {
			System.out.println(
					"Podaj numer portu jako parametr (provide port number in command line)"
					);
			return;
		}
		try {
			c5p1a server = new c5p1a(
					Integer.parseInt(args[0]));
			System.out.println("Czekam (I am waiting)...");
			server.dataExchange();

			// ew. wyjatek wyrzucany przez konstruktor
			// (exception throwed by the constructor)
			// c5p1a
		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
