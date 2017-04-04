import java.io.*;
import java.net.*;

public class c5p1b {

    // args[0] - nazwa hosta (lub IP) (hostname)
    // args[1] - numer portu (port number)
    public static void main(String[] args) {
        if (args.length < 2) {
            System.out.println(
                "Podaj nazwe hosta i numer portu"
            );
            return;
        }
        try {
            // utworzenie gniazda - lokalny port niewazny (socket creation on "any" port)
            DatagramSocket socket = new DatagramSocket();

            // ustalenie adresu na podstawie args[0] (address from args[0])
            InetAddress addr = InetAddress.getByName(args[0]);

            // utworzenie datagramu z wiadomoscia (creation of datagram with message)
            String s = "SIK 420";
            DatagramPacket p = new DatagramPacket(
                s.getBytes(), s.length(),
                addr, Integer.parseInt(args[1]));

            // wyslanie wiadomosci (message sending)
            socket.send(p);

            // utworzenie "pustego" datagramu (creation of empty datagram)
            byte[] bufor = new byte[256];
            DatagramPacket p2 = new DatagramPacket(bufor, 256);

            // czekaj na wiadomosc zwrotna (wait for reply message)
            socket.receive(p2);

            // utworz lancuch z tablicy bajtow (create string from array of bytes)
            String response = new String(p2.getData());

            // wyswietl wiadomosc zwrotna (print reply message)
            System.out.println("Odpowiedź z: "+p2.getAddress().getHostName());
            System.out.println("Serwer powiedzial (server said): "+response);

            // koniec protokolu (end of protocol)
            socket.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
