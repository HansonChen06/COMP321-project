import java.util.*;

public class Main {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        int n = sc.nextInt();
        int m = sc.nextInt();

        long[] guess = new long[n + 1];
        for (int i = 1; i <= n; i++) guess[i] = sc.nextLong();

        ArrayList<int[]>[] graph = new ArrayList[n + 1];
        for (int i = 1; i <= n; i++) graph[i] = new ArrayList<>();

        for (int i = 0; i < m; i++) {
            int a = sc.nextInt();
            int b = sc.nextInt();
            int w = sc.nextInt();

            graph[a].add(new int[]{b, w});
            graph[b].add(new int[]{a, w});
        }

        long[] dist = new long[n + 1];
        Arrays.fill(dist, Long.MAX_VALUE);
        dist[n] = 0;

        PriorityQueue<long[]> heap = new PriorityQueue<>(
                (a, b) -> Long.compare(a[0], b[0])
        );

        heap.add(new long[]{0, n});

        while (!heap.isEmpty()) {
            long[] cur = heap.poll();

            long d = cur[0];
            int c_node = (int) cur[1];

            if (d != dist[c_node]) continue;

            for (int[] edge : graph[c_node]) {
                int next_node = edge[0];
                int w = edge[1];

                long new_dist = d + w;

                if (new_dist < dist[next_node]) {
                    dist[next_node] = new_dist;
                    heap.add(new long[]{new_dist, next_node});
                }
            }
        }

        System.out.println(dist[1]);

        for (int i = 1; i <= n; i++) {
            if (guess[i] != dist[i]) {
                System.out.println(i + " " + dist[i]);
            }
        }

    }
}