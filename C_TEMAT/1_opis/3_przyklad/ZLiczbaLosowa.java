public class ZLiczbaLosowa
    implements Zadanie { //Task

    public Object Wykonaj(Object params) {
        System.out.println("*** Tu zadanie liczba losowa (Task random number is here).");
        return new Double(Math.random());
    }
} 
