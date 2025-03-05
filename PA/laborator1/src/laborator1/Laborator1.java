/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/Classes/Main.java to edit this template
 */
package laborator1;

/**
 *
 * @author Cazacu Denisa
 */
class Graph {
    private int n;
    private int k;
    private int m;
    private int[][] adjMatrix;

    public Graph() {
    }

    public Graph(int n, int k) {
        this.n = n;
        this.k = k;
        adjMatrix = new int[n][n];
    }

    public void setN(int n) {
        this.n = n;
        adjMatrix = new int[n][n];
    }

    public void setK(int k) {
        this.k = k;
    }

    public int getN() {
        return n;
    }

    public int getK() {
        return k;
    }

    public void generateMatrix() {
        int[] cliqueVertices = new int[k];
        int[] stableSet = new int[k];
        int[] usedVertices = new int[n];
        boolean usedVertexFromClique = false; // we can use at most one vertex from the clique in the stable set

        if (k > (n + 1) / 2) {
            System.out.println("The graph can't have a clique and a stable set of size " + k);
            int randomEdges = (int) (Math.random() * (n * n));
            for (int i = 0; i <= randomEdges; i++) {
                int randomVertex1 = (int) (Math.random() * n);
                int randomVertex2 = (int) (Math.random() * n);
                adjMatrix[randomVertex1][randomVertex2] = 1;
                adjMatrix[randomVertex2][randomVertex1] = 1;
            }
        } else {
            // generate some random vertices for the clique
            for (int i = 0; i < k; i++) {
                int randomVertex = (int) (Math.random() * n);
                while (usedVertices[randomVertex] != 0) {
                    randomVertex = (int) (Math.random() * n);
                }
                usedVertices[randomVertex] = 1;
                cliqueVertices[i] = randomVertex;
            }
            //add the edges of the clique
            for (int i = 0; i < k; i++) {
                for (int j = i + 1; j < k; j++) {
                    adjMatrix[cliqueVertices[i]][cliqueVertices[j]] = 1;
                    adjMatrix[cliqueVertices[j]][cliqueVertices[i]] = 1;
                }
            }
            //generate random vertices for the stable set
            for (int i = 0; i < k; i++) {
                int randomVertex = (int) (Math.random() * n);
                while (usedVertices[randomVertex] != 0) {
                    randomVertex = (int) (Math.random() * n);
                    if (usedVertices[randomVertex] == 1 && !usedVertexFromClique) {
                        usedVertexFromClique = true;
                        break;
                    }
                }
                usedVertices[randomVertex] = 1;
                stableSet[i] = randomVertex;
            }

            //try to generate random edges and add them only if the vertices are not in the stable set
            int randomEdges = (int) (Math.random() * (n * n));
            for (int i = 0; i <= randomEdges; i++) {
                int randomVertex1 = (int) (Math.random() * n);
                int randomVertex2 = (int) (Math.random() * n);
                boolean found1 = false;
                boolean found2 = false;
                for (int j = 0; j < k; j++) {
                    if (stableSet[j] == randomVertex1) {
                        found1 = true;
                    }
                    if (stableSet[j] == randomVertex2) {
                        found2 = true;
                    }
                }
                if (!(found1 && found2) && randomVertex1 != randomVertex2) {
                    adjMatrix[randomVertex1][randomVertex2] = 1;
                    adjMatrix[randomVertex2][randomVertex1] = 1;
                }
            }
        }
    }
    public void printMatrix() {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                System.out.print(adjMatrix[i][j] + " ");
            }
            System.out.println();
        }
    }
    public String matrixToString() {
        StringBuilder s = new StringBuilder();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (adjMatrix[i][j] == 0) {
                    s.append("\u25CC").append(" ");
                }
                else{
                    s.append("\u25C9").append(" ");
                }
            }
            s.append("\n");
        }
        return s.toString();
    }

    public void computeM(){
        m = 0;
        for (int i = 0; i < n; i++) {
            for (int j = i + 1; j < n; j++) {
                if (adjMatrix[i][j] == 1) {
                    m++;
                }
            }
        }
    }
    public int getM(){
        return m;
    }
    public void displayMinMaxDegree() {
        int min = n+1;
        int max = 0;
        int[] degrees = new int[n];
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                degrees[i] += adjMatrix[i][j];
            }
            if (degrees[i] > max) {
                max = degrees[i];
            }
            if (degrees[i] < min) {
                min = degrees[i];
            }
        }
        System.out.println("The maximum degree \u2206 is " + max);
        System.out.println("The minimum degree \u03B4 is " + min);
    }
    public boolean verifySum(){
        int sum = 0;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                sum += adjMatrix[i][j];
            }
        }
        if (sum == m * 2){
            return true;
        }
        return false;
    }
}

public class Laborator1 {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) {
        Laborator1 lab1 = new Laborator1();
        lab1.compulsory();
        lab1.homework(args);
    }

    void compulsory() {
        String[] languages =  {"C", "C++", "C#", "Python", "Go", "Rust", "JavaScript", "PHP", "Swift", "Java"};
        int n = (int) (Math.random() * 1_000_000);
        n = n*3;
        n = n+ 0b10101;
        n = n+ 0xFF;
        n = n*6;
        int result=0;
        while (n>0){
            result = result + n%10;
            n=n/10;
        }
        while (result>9){
            int aux=0;
            while (result>0){
                aux = aux + result % 10;
                result = result/10; 
            }
            result = aux;
        }
        String resultedString = "Willy-nilly, this semester I will learn " + languages[result];
        System.out.println(resultedString);
     }
    void homework(String[] args) {
        if (args.length != 2) {
            System.out.println(
            "Usage: number, number");
            System.exit(-1);
        }
        int n = Integer.parseInt(args[0]);
        int k = Integer.parseInt(args[1]);
        long t0 = System.currentTimeMillis();
        Graph G = new Graph(n, k);
        G.generateMatrix();
        String matrixString = G.matrixToString();
        if (n<100) {
            System.out.println("\n" + matrixString);
        }
        G.computeM();
        System.out.println("Number of edges: " + G.getM());
        G.displayMinMaxDegree();
        System.out.println("Is the sum of the degrees equal with m*2 ? " + G.verifySum() );
        long t1 = System.currentTimeMillis();
        System.out.println(t1 - t0);
    }
     void bonus() {
    //Do stuff
     }

}
