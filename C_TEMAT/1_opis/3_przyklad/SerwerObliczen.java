import java.net.*;
import java.io.*;

public class SerwerObliczen { //ComputationServer
    private int port;
    private ServerSocket ss;

    public SerwerObliczen(int aport) {
        super();
        port = aport;
        ss = null;
    }

    public void InicjujGniazdo() { //InitializeSocket
        try {
            ss = new ServerSocket(port);
        } catch (Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
    }

    public void WykonujZadania() { //ExecuteTasks
        Socket s;
        ObjectInputStream ois;
        ObjectOutputStream oos;

        while (true) {
            System.out.println("Czekam na zadanie (waiting for a task)...");
            try {
                s = ss.accept();
                System.out.println("Polaczenie z (connection from) "+
                    s.getInetAddress().getHostName());
                System.out.println("Odczytywanie zadania (task reading) ...");
                oos = new ObjectOutputStream(s.getOutputStream());
                ois = new ObjectInputStream(s.getInputStream());
                Zadanie z = (Zadanie) ois.readObject();
                System.out.println("Odebrano zadanie typu (received task of the type)"+
                    z.getClass().getName());
                System.out.println(
                    "Odczytywanie parametrow (reading parameters)...");
                Object par = ois.readObject();
                System.out.println(
                    "Parametry odczytane. Wykonuje zadanie (Parameters read. I am executing a task)...");
                Object wynik = z.Wykonaj(par);
                System.out.println(
                    "Zadanie wykonane. Wysylam wyniki (task done, sending results)...");
                oos.writeObject(wynik);
                System.out.println("Gotowe. Zamykam sesje z (ready, I am closing session with) "+
                    s.getInetAddress().getHostName());
                ois.close();
                oos.close();
                s.close();
            } catch(Exception e) {
                e.printStackTrace();
                System.exit(1);
            }
        }
    }
    public static void main(String[] args) {
        int port = 0;
        BufferedReader klawiatura; //keyboard

        try {
            klawiatura = new BufferedReader(
                new InputStreamReader(
                    System.in
                    )
                );
            System.out.print("Podaj numer portu (provide port number): ");
            port = Integer.parseInt(
                klawiatura.readLine()
                );

        } catch(Exception e) {
            e.printStackTrace();
            System.exit(1);
        }
        SerwerObliczen serwer = new SerwerObliczen(port);
        serwer.InicjujGniazdo();
        serwer.WykonujZadania();
    }
}
