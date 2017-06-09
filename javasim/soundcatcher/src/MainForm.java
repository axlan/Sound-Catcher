/**
 * Created by jdiamond on 6/6/2017.
 */

import javax.sound.sampled.*;
import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.File;
import java.io.IOException;
import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;

public class MainForm {

    public static void main(String[] args) {
        SwingUtilities.invokeLater(new Runnable() {
            public void run() {
                createAndShowGUI();
            }
        });
    }

    private static void createAndShowGUI() {
        System.out.println("Created GUI on EDT? "+
                SwingUtilities.isEventDispatchThread());
        JFrame f = new JFrame("Swing Paint Demo");
        f.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        f.add(new MyPanel());
        f.pack();
        f.setSize(800, 800);
        f.setVisible(true);
    }
}

class TestPattern1 {
    private int spoke_idx=0;
    private int led_idx=0;
    private MyPanel panel;

    TestPattern1(MyPanel panel) {
        this.panel = panel;
    }

    void connect() {
        spoke_idx = 0;
        led_idx = 0;
        int timerDelay = 500;
        new Timer(timerDelay, new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                panel.setled(Color.black, spoke_idx, led_idx);
                led_idx++;
                if (led_idx == MyPanel.NUM_LED_PER_SPOKE) {
                    led_idx = 0;
                    spoke_idx = (spoke_idx+1) % MyPanel.NUM_SPOKES;
                }
                panel.setled(Color.red, spoke_idx, led_idx);
                panel.repaint();
            }
        }).start();
    }
}


class TestPattern2 {
    private int spoke_idx=0;
    private MyPanel panel;

    TestPattern2(MyPanel panel) {
        this.panel = panel;
    }

    void connect() {
        spoke_idx = 0;
        int timerDelay = 500;
        new Timer(timerDelay, new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                panel.setspoke(Color.black, spoke_idx);
                spoke_idx = (spoke_idx+1) % MyPanel.NUM_SPOKES;
                panel.setspoke(Color.red, spoke_idx);
                panel.repaint();
            }
        }).start();
    }
}

class Clock1 {
    private MyPanel panel;
    public Clock1(MyPanel panel) {
        this.panel = panel;
    }
    private int hour2spoke(int hour) {
        return (hour + 9) % 12;
    }
    private int min2spoke(int min) {
        return ((min + 45) % 60) / 5;
    }
    public void connect() {
        int timerDelay = 1000;
        new Timer(timerDelay, new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                panel.setall(Color.black);
                GregorianCalendar now = new GregorianCalendar();
                now.setTimeInMillis(System.currentTimeMillis());

                panel.setspoke(Color.red, hour2spoke(now.get(Calendar.HOUR)));
                panel.setspoke(Color.blue, min2spoke(now.get(Calendar.MINUTE)));
                panel.setspoke(Color.green, min2spoke(now.get(Calendar.SECOND)));

                panel.repaint();
            }
        }).start();
    }
}

class Clock2 {
    private MyPanel panel;
    public Clock2(MyPanel panel) {
        this.panel = panel;
    }
    private static int hour2spoke(int hour) {
        return (hour + 9) % 12;
    }
    private static int min2spoke(int min) {
        return ((min + 45) % 60) / 5;
    }

    private void drawsecond(double second) {
        int spoke = min2spoke((int)second);
        double progress = second / 5.0 - Math.floor(second / 5.0);
        panel.setspoke(Color.green, spoke, progress);
    }

    public void connect() {
        int timerDelay = 100;
        new Timer(timerDelay, new ActionListener(){
            public void actionPerformed(ActionEvent e) {
                panel.setall(Color.black);
                GregorianCalendar now = new GregorianCalendar();
                now.setTimeInMillis(System.currentTimeMillis());

                panel.setspoke(Color.red, hour2spoke(now.get(Calendar.HOUR)));
                panel.setspoke(Color.blue, min2spoke(now.get(Calendar.MINUTE)));

                drawsecond(now.get(Calendar.SECOND) + now.get(Calendar.MILLISECOND) / 1000.0);
                panel.repaint();
            }
        }).start();
    }
}

abstract class SoundBase {

    protected MyPanel panel;
    private File file;
    private long start_time;
    private long cur_sample;
    protected int NFFT = 32;
    private final int BUF_SIZE = 1024;
    private WavFile wavFile;
    protected int timerDelay = 100;
    protected boolean blank = true;

    public SoundBase(MyPanel panel, String file_path) {
        this.panel = panel;
        file = new File(file_path);
    }


    public abstract void display(double mag[]);

    public void connect() {
        cur_sample = 0;

        try {
            AudioInputStream audioInputStream = AudioSystem.getAudioInputStream(file);
            Clip clip = AudioSystem.getClip();
            clip.open(audioInputStream);
            clip.start();
        } catch (UnsupportedAudioFileException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (LineUnavailableException e) {
            e.printStackTrace();
        }

        try {
            start_time = System.currentTimeMillis() + 200;
            wavFile = WavFile.openWavFile(file);
            // Display information about the wav file
            wavFile.display();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (WavFileException e) {
            e.printStackTrace();
        }


        new Timer(timerDelay, new ActionListener(){
            public void actionPerformed(ActionEvent e) {

                try {


                    // Get the number of audio channels in the wav file
                    int numChannels = wavFile.getNumChannels();
                    double sample_rate = wavFile.getSampleRate();

                    long new_sample = (int)(((double)(System.currentTimeMillis() - start_time)) * sample_rate / 1000.0);

                    // Create a buffer of 100 frames
                    double[] buffer = new double[BUF_SIZE * numChannels];

                    int framesRead;
                    double min = Double.MAX_VALUE;

                    while (cur_sample < new_sample) {

                        // Read frames into buffer
                        framesRead = wavFile.readFrames(buffer, BUF_SIZE);
                        if (framesRead!=BUF_SIZE) {
                            System.out.println("EOF");
                            return;
                        }

                        cur_sample += BUF_SIZE / numChannels;

                    }


                    Complex x[] = new Complex[NFFT];
                    for (int i = 0; i < NFFT; i++) {
                        x[i] = new Complex(buffer[i*numChannels], 0);
                    }
                    Complex Y[] = FFT.fft(x);


/*
                Loader.load(fftw3.class);

                DoublePointer signal = new DoublePointer(NFFT);
                DoublePointer result = new DoublePointer(NFFT);

                fftw3.fftw_plan plan = fftw_plan_dft_r2c_1d(NFFT, signal, result,
                        (int)FFTW_PATIENT);

                signal.put(s);

                fftw_execute(plan);

                fftw_destroy_plan(plan);
*/
                    double mag[] = new double[NFFT / 2];
                    for(int i = 0; i < MyPanel.NUM_SPOKES && i < NFFT/2; i++) {
                        mag[i] = Y[i].abs();
                    }
                    if(blank) {
                      panel.setall(Color.black);
                    }
                    display(mag);
                    panel.repaint();

                /*
                double results[] = new double[NFFT/2];
                for(int i = 0; i < NFFT/2; i++) {
                    results[i] = Y[i].abs();
                    System.out.print(""+results[i]+",");
                }
                System.out.println();
                */

                } catch (IOException e1) {
                    e1.printStackTrace();
                } catch (WavFileException e1) {
                    e1.printStackTrace();
                }
            }
        }).start();
    }

}

class Sound1 extends SoundBase {

    public Sound1(MyPanel panel, String file_path) {
        super(panel, file_path);
    }

    public void display(double mag[]) {
        for(int i = 0; i < MyPanel.NUM_SPOKES && i < NFFT/2; i++) {
            double percent = (Math.log(mag[i]) + 2 ) / 4;

            panel.setspoke(Color.blue, i, percent);
        }
    }
}

class Sound2 extends SoundBase {

    double spoke_hat[];

    public Sound2(MyPanel panel, String file_path) {
        super(panel, file_path);
        spoke_hat = new double [MyPanel.NUM_SPOKES];
    }

    public void display(double mag[]) {
        for(int i = 0; i < MyPanel.NUM_SPOKES && i < NFFT/2; i++) {
            double percent = (Math.log(mag[i]) + 2 ) / 4;

            spoke_hat[i] -= .01;
            spoke_hat[i] = Math.max(spoke_hat[i], percent);

            panel.setspoke(Color.blue, i, percent);

            panel.setled(Color.red, i, panel.percent_to_led(spoke_hat[i]));

        }
    }
}

class Sound3 extends SoundBase {

    double spoke_hat[];

    public Sound3(MyPanel panel, String file_path) {
        super(panel, file_path);
        spoke_hat = new double [MyPanel.NUM_SPOKES];
    }

    public void display(double mag[]) {
        for(int i = 0; i < MyPanel.NUM_SPOKES && i < NFFT/2; i++) {
            double percent = (Math.log(mag[i]) + 2 ) / 4;

            spoke_hat[i] -= .01;
            spoke_hat[i] = Math.max(spoke_hat[i], percent);

            int max_led = panel.percent_to_led(percent);
            for (int j =0; j <= max_led; j++) {
                Color color = new Color(j*35, 0, 60);
                panel.setled(color, i, j);
            }

            panel.setled(Color.red, i, panel.percent_to_led(spoke_hat[i]));

        }
    }
}


class Sound4 extends SoundBase {

    double ring_hat[];

    public Sound4(MyPanel panel, String file_path) {
        super(panel, file_path);
        ring_hat = new double [MyPanel.NUM_LED_PER_SPOKE];
        NFFT = 16;
    }

    public void display(double mag[]) {
        for(int i = 0; i < MyPanel.NUM_LED_PER_SPOKE && i < NFFT/2; i++) {
            double percent = (Math.log(mag[i]) + 2 ) / 4;

            ring_hat[i] -= .01;
            ring_hat[i] = Math.max(ring_hat[i], percent);

            int max_led = panel.percent_to_ring(percent);
            for (int j =0; j <= max_led; j++) {
                Color color = new Color(j*20, 0, 60);
                panel.setled(color, j, i);
            }

            panel.setled(Color.red, panel.percent_to_ring(ring_hat[i]), i);

        }
    }
}


class Sound5 extends SoundBase {

    final Color colors[] = {Color.green, Color.PINK, Color.BLUE, Color.yellow, Color.RED, Color.CYAN, Color.magenta, Color.orange};

    public Sound5(MyPanel panel, String file_path) {
        super(panel, file_path);
        NFFT = 16;
    }

    public void display(double mag[]) {
        for(int i = 0; i < MyPanel.NUM_LED_PER_SPOKE && i < NFFT/2; i++) {
            double percent = (Math.log(mag[i]) + 2 ) / 4;
            panel.setring(colors[i], i, percent);
        }
    }
}



class Sound6 extends SoundBase {

    double ring_hat[];

    final Color colors[] = {Color.green, Color.PINK, Color.BLUE, Color.yellow, Color.RED, Color.CYAN, Color.magenta, Color.orange};

    public Sound6(MyPanel panel, String file_path) {
        super(panel, file_path);
        ring_hat = new double [MyPanel.NUM_LED_PER_SPOKE];
        NFFT = 16;
    }

    public void display(double mag[]) {
        for(int i = 0; i < MyPanel.NUM_LED_PER_SPOKE && i < NFFT/2; i++) {
            double percent = (Math.log(mag[i]) + 2 ) / 6;

            ring_hat[i] -= .01;
            ring_hat[i] = Math.max(ring_hat[i], percent);

            panel.setring(colors[i], i, ring_hat[i]);
        }
    }
}

class Sound7 extends SoundBase {

    public Sound7(MyPanel panel, String file_path) {
        super(panel, file_path);
        NFFT = 32;
        timerDelay = 100;
        blank = false;
    }

    public void display(double mag[]) {


        double total_percent;
        double r=0,g=0,b = 0;


        for (int i = 0; i < 5; i++) {
            r+=mag[i+10];
            g+=mag[i+5];
            b+=mag[i];
        }

        double r_percent = (Math.log(r) + 2 ) / 4;
        double g_percent = (Math.log(g) + 2 ) / 4;
        double b_percent = (Math.log(b) + 2 ) / 6;

        total_percent = (r_percent+g_percent+b_percent) / 3;

        Color color = new Color((int)(255 * Math.max(0,Math.min(1, r_percent))),
                                (int)(255 * Math.max(0,Math.min(1, g_percent))),
                                (int)(255 * Math.max(0,Math.min(1, b_percent))));

        panel.setspoke(Color.red,MyPanel.NUM_SPOKES-1);
        panel.setspoke(Color.black,0);
        panel.setspoke(color,0, total_percent);


        panel.rotate_spokes_cw();
    }
}


class MyPanel extends JPanel {

    private int squareX = 50;
    private int squareY = 50;
    private int squareW = 20;
    private int squareH = 20;

    private final int CONSTANT_OFFSET = 400;
    private final int CENTER_OFFSET = 40;
    private final int LED_SPACING = 40;
    private final int LED_SIZE = 10;
    static final int NUM_SPOKES = 12;
    static final int NUM_LED_PER_SPOKE = 8;

    private int spoke_idx;
    private int led_idx;

    private Color spokes[][];

    MyPanel() {

        setBorder(BorderFactory.createLineBorder(Color.black));
        setDoubleBuffered(true);
        setBackground(Color.black);

        setall(Color.black);

/*
        addMouseListener(new MouseAdapter() {
            public void mousePressed(MouseEvent e) {
                moveSquare(e.getX(),e.getY());
            }
        });

        addMouseMotionListener(new MouseAdapter() {
            public void mouseDragged(MouseEvent e) {
                moveSquare(e.getX(),e.getY());
            }
        });
  */

        //Clock2 pattern = new Clock2(this);
        String path = "C:\\Users\\axlan\\Downloads\\Black Moth Super Rainbow - 09 The Afternoon Turns Pink.wav";
        //String path = "C:\\Users\\axlan\\Downloads\\Various Artists - 017 - Queen - Bohemian Rhapsody.wav";
        //String path = "C:\\Users\\axlan\\Downloads\\Various Artists - 001 - Led Zeppelin - Stairway To Heaven.wav";
        Sound7 pattern = new Sound7(this, path);
        pattern.connect();

    }

    void rotate_spokes_cw() {
        Color tmp[] = spokes[MyPanel.NUM_SPOKES-1];
        for(int i = MyPanel.NUM_SPOKES-1; i > 0; i--) {
            spokes[i] = spokes[i-1];
        }
        spokes[0] = tmp;
    }

    int percent_to_ring(double percent) {
        return (int)Math.max(0,
                Math.min(NUM_SPOKES - 1,
                        Math.round(percent * (double)(NUM_SPOKES)) - 1));
    }

    int percent_to_led(double percent) {
        return (int)Math.max(0,
                             Math.min(NUM_LED_PER_SPOKE - 1,
                                      Math.round(percent * (double)(NUM_LED_PER_SPOKE)) - 1));
    }

    void setring(Color c, int ring) {
        for (int j = 0; j < NUM_SPOKES; j++) {
            setled(c, j, ring);
        }
    }

    void setring(Color c, int band, double percent) {
        int leds = percent_to_ring(percent);
        for(int i = 0; i <= leds; i++) {
            setled(c, i, band);
        }
    }

    void setled(Color c, int spoke, int led) {
        spokes[spoke][led] = c;
    }

    void setspoke(Color c, int spoke) {
        for (int j = 0; j < NUM_LED_PER_SPOKE; j++) {
            setled(c, spoke, j);
        }
    }

    void setspoke(Color c, int spoke, double percent) {
        int leds = percent_to_led(percent);
        for(int i = 0; i <= leds; i++) {
            setled(c, spoke, i);
        }
    }

    void setall(Color c) {
        spokes = new Color[NUM_SPOKES][NUM_LED_PER_SPOKE];
        for (int i = 0; i < NUM_SPOKES; i++) {
            setspoke(c, i);
        }
    }

    public Dimension getPreferredSize() {
        return new Dimension(250,200);
    }

    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        /*
        g.drawString("This is my custom Panel!",10,20);
        g.setColor(Color.RED);
        g.fillRect(squareX,squareY,squareW,squareH);
        g.setColor(Color.BLACK);
        g.drawRect(squareX,squareY,squareW,squareH);*/
        double angle = 0;
        for (Color[] spoke: spokes) {
            int offset = CENTER_OFFSET;
            for (Color led: spoke) {
                int x = (int)Math.round(offset * Math.cos(angle)) + CONSTANT_OFFSET;
                int y = (int)Math.round(offset * Math.sin(angle)) + CONSTANT_OFFSET;
                g.setColor(led);
                g.fillOval(x, y, LED_SIZE, LED_SIZE);
                offset += LED_SPACING;
            }
            angle += 2 * Math.PI / NUM_SPOKES;
        }
    }
}