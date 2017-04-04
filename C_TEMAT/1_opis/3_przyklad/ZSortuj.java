 public class ZSortuj
    implements Zadanie { //Task

    public Object Wykonaj(Object params) { //execute
        int[] tablica = (int[]) params; //array
        int i, j;
        System.out.println("*** Tu zadanie sortowania (task sort is here).");
        for (i=1; i<tablica.length; i++) {
            for (j=0; j<tablica.length-1; j++) {
                if (tablica[j]>tablica[j+1]) {
                    int tmp = tablica[j];
                    tablica[j] = tablica[j+1];
                    tablica[j+1] = tmp;
                }
            }
        }
        return tablica;
    }
} 
