import java.io.*;
import java.net.*;

public class c5p2b {

    // args[0] - nazwa hosta (hostname)
    // args[1] - numer portu (port number)
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println(
                "Podaj nazwe hosta i port (provide hostname and port number)"
            );
            return;
        }
        try {
            // ustal adres serwera (obtain host address)
            InetAddress addr = InetAddress.getByName(args[0]);

            // ustal port (set port)
            int port = Integer.parseInt(args[1]);

            // utworz gniazdo i od razu podlacz je
            // do addr:port
        // create socket and bind it to addr:port
            Socket socket = new Socket(addr, port);

            // pobierz strumienie i zbuduj na nich
            // "lepsze" strumienie
        // make "better" streams
            DataOutputStream dos = new DataOutputStream(
                socket.getOutputStream());
            DataInputStream dis = new DataInputStream(
                socket.getInputStream());

            // zapisz kolejno int, String i double
        // write int, String, double
            dos.writeInt(1000);
            dos.writeUTF("Hello World!");
            dos.writeDouble(3.14159);

            // czytaj odpowiedz (wait for answer)
            String s = dis.readUTF();

            // wypisz odpowiedz (print and reply)
            System.out.println("Serwer powiedzial (server said): "+s);
            dis.close();
            dos.close();

            // koniec rozmowy (end of conversation)
            socket.close();

        // moga byc wyjatki dot. gniazd,
        // getByName, parseInt i strumieni
    // possible exception (from getByName, parseInt and streams)
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("Klient zakonczyl dzialanie (klient finished)");
    }
}
