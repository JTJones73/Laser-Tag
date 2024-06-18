import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.text.Collator;
import java.util.Collections;
import java.util.Comparator;
import java.util.LinkedList;

class PlayerScore
{
    public int Score;
    public String Name;
};

public class Main extends JFrame {
    private JLabel team1Label;
    private JLabel team2Label;
    private int team1Score;
    private int team2Score;
    private Timer timer;
    private static JLabel[] redPlayerSlot= new JLabel[]{new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel("")};
    private static JLabel[] bluePlayerSlot= new JLabel[]{new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel("")};
    private static JLabel[] redScoreSlot= new JLabel[]{new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel("")};
    private static JLabel[] blueScoreSlot= new JLabel[]{new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel(""),new JLabel("")};

    static final int NUM_PLAYERS_PER_TEAM = 16;
    public static LinkedList<PlayerScore> ScoreBoard = new LinkedList<PlayerScore>();

    public Main() {
        setUndecorated(false); // Remove window decorations
        //setExtendedState(JFrame.MAXIMIZED_BOTH); // Maximize window to full screen
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Set background color of JFrame
        getContentPane().setBackground(Color.BLACK);

        // Initialize scores
        team1Score = 0;
        team2Score = 0;

        JPanel panel = new JPanel();
        panel.setLayout(new GridLayout(3, 1));
        panel.setBackground(Color.BLACK);

        // Labels for team scores
        team1Label = new JLabel("Team 1: \n" + team1Score);
        team1Label.setForeground(Color.WHITE);
        team1Label.setFont(new Font("Arial", Font.PLAIN, 48)); // Larger font

        team2Label = new JLabel("Team 2: \n\n\t" + team2Score);
        team2Label.setForeground(Color.WHITE);
        team2Label.setFont(new Font("Arial", Font.PLAIN, 48)); // Larger font

        // Add labels to panel
        panel.add(new JLabel()); // Empty label for spacing
        panel.add(team1Label);
        panel.add(team2Label);

        // Add panel to frame
        add(panel);


        // Create a Timer to update scores every 100ms
        timer = new Timer(100, new ActionListener() {
            @Override
            public void actionPerformed(ActionEvent e) {
                // Update labels with current scores
                team1Label.setText("Team 1: \n" + team1Score);
                team2Label.setText("Team 2: \n\n\t" + team2Score);
            }
        });

        // Start the timer
        timer.start();

        // Display the frame
        setVisible(true);

        // Open a command line window
        openCommandLine();
    }

    private void openCommandLine() {
        try {
            // Open command line / terminal window
            ProcessBuilder pb = new ProcessBuilder("cmd", "/c", "start", "cmd.exe");
            Process p = pb.start();

            // Redirect input/output streams of command line
            PrintWriter pw = new PrintWriter(p.getOutputStream(), true);
            BufferedReader br = new BufferedReader(new InputStreamReader(p.getInputStream()));

            // Continuously read commands from command line
            new Thread(new Runnable() {
                @Override
                public void run() {
                    String line;
                    try {
                        while ((line = br.readLine()) != null) {
                            processCommand(line, pw);
                        }
                    } catch (IOException e) {
                        e.printStackTrace();
                    }
                }
            }).start();
        } catch (IOException ex) {
            ex.printStackTrace();
        }
    }

    private void processCommand(String command, PrintWriter pw) {
        // Example command: setscore:10
        String[] tokens = command.split(":");
        if (tokens.length == 2 && tokens[0].equals("setscore")) {
            int score;
            try {
                score = Integer.parseInt(tokens[1]);
            } catch (NumberFormatException e) {
                pw.println("Invalid score format: " + tokens[1]);
                return;
            }

            // Update scores for both teams
            team1Score = score;
            team2Score = score;


            SwingUtilities.invokeLater(new Runnable() {
                @Override
                public void run() {
                    Collections.sort(ScoreBoard, new Comparator<PlayerScore>() {
                        @Override
                        public int compare(PlayerScore o1, PlayerScore o2) {
                            return Collator.getInstance().compare(o1.Score, o2.Score);
                        }
                    });
                    team1Label.setText("Team 1: \n" + team1Score);
                    team2Label.setText("Team 2: \n\n\t" + team2Score);
                }
            });

            pw.println("Scores updated successfully.");
        } else {
            pw.println("Invalid command format. Example usage: setscore:10");
        }
    }

    public static void main(String[] args)
    {
        // Create a new JFrame
        // Create a new JFrame
        JFrame frame = new JFrame("My First JFrame");

        // Set null layout for absolute positioning
        frame.setLayout(null);

        // Create labels
        JLabel titleLabel = new JLabel("Laser Tag!");
        JLabel redTeamLabel = new JLabel("Red Team");
        JLabel blueTeamLabel = new JLabel("Blue Team");
        JLabel player1 = new JLabel("Scardeyccat");
        JLabel player2 = new JLabel("Theencomputers ");
        JLabel p1Score = new JLabel("1337");
        JLabel p2Score = new JLabel("69420");
        // Set fonts (optional)
        titleLabel.setFont(new Font("Courier", Font.BOLD, 72));
        redTeamLabel.setFont(new Font("Courier", Font.BOLD, 64));
        blueTeamLabel.setFont(new Font("Courier", Font.BOLD, 64));


        // Set positions for each label
        titleLabel.setBounds(1350, 50, 1000, 100); // x, y, width, height
        redTeamLabel.setBounds(300, 200, 1000, 100);
        blueTeamLabel.setBounds(2300, 200, 1000, 100);

        redTeamLabel.setForeground(Color.WHITE);
        blueTeamLabel.setForeground(Color.WHITE);
        titleLabel.setForeground(Color.WHITE);

        frame.getContentPane().setBackground(Color.BLACK);
        // Add labels to the frame
        frame.add(titleLabel);
        frame.add(redTeamLabel);
        frame.add(blueTeamLabel);
        for(int i = 0; i < NUM_PLAYERS_PER_TEAM; i++){
            redPlayerSlot[i].setForeground(Color.WHITE);
            redPlayerSlot[i].setFont(new Font("Courier", Font.BOLD, 48));
            frame.add(redPlayerSlot[i]);
            redScoreSlot[i].setForeground(Color.WHITE);
            redScoreSlot[i].setFont(new Font("Courier", Font.BOLD, 48));
            frame.add(redScoreSlot[i]);
            redPlayerSlot[i].setBounds(50, 300 + i*100, 1000, 100);
            redScoreSlot[i].setBounds(700, 300 + i*100, 1000, 100);

            bluePlayerSlot[i].setForeground(Color.WHITE);
            bluePlayerSlot[i].setFont(new Font("Courier", Font.BOLD, 48));
            frame.add(bluePlayerSlot[i]);
            blueScoreSlot[i].setForeground(Color.WHITE);
            blueScoreSlot[i].setFont(new Font("Courier", Font.BOLD, 48));
            frame.add(blueScoreSlot[i]);
            bluePlayerSlot[i].setBounds(1975, 300 + i*100, 1000, 100);
            blueScoreSlot[i].setBounds(2750, 300 + i*100, 1000, 100);

        }


        // Set frame size based on the components' positions
        int width = 500; // Adjust based on your content
        int height = 250; // Adjust based on your content
        frame.setSize(width, height);

        // Center the frame on the screen
        Dimension screenSize = Toolkit.getDefaultToolkit().getScreenSize();
        int x = (screenSize.width - width) / 2;
        int y = (screenSize.height - height) / 2;
        frame.setLocation(x, y);

        // Set default close operation
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

        // Make the frame visible
        frame.setVisible(true);
        setScoreboard(new String[]{"lolz", "Van"},new String[]{"100", "0"},new String[]{"n12rs2", "tntcastle"},new String[]{"10", "23"});
    }
    public static void setScoreboard(String[] redTeamPlayers, String[] redTeamScore, String[] blueTeamPlayers, String[] blueTeamScore){
        for(int i = 0; i < redTeamPlayers.length; i++){
            redPlayerSlot[i].setText(redTeamPlayers[i]);
            redScoreSlot[i].setText(redTeamScore[i]);
        }
        for(int i = 0; i < blueTeamPlayers.length; i++){
            bluePlayerSlot[i].setText(blueTeamPlayers[i]);
            blueScoreSlot[i].setText(blueTeamScore[i]);
        }
    }
}
