/* Start reading here */

import org.bytedeco.javacpp.*;

import java.io.File;
import java.io.IOException;

import static java.lang.Math.*;
import static org.bytedeco.javacpp.fftw3.*;

public class FFTWExample {

    static final int NUM_POINTS = 64;


    /* Never mind this bit */

    static final int REAL = 0;
    static final int IMAG = 1;

    static double sample_rate;

    static void acquire_from_somewhere(DoublePointer signal) {
        /* Generate two sine waves of different frequencies and amplitudes. */
        double[] s = new double[(int)signal.capacity()];

        /*
        sample_rate = 100;
        for (int i = 0; i < NUM_POINTS; i++) {
            double t = (double)i / sample_rate;
            s[i] = 1.0 * cos(20.3125 * t * (2.0 * PI));
        }*/

        final int BUF_SIZE = 10240;
        final double TIME_OFFSET = 5;
        File file = new File("C:\\Users\\jdiamond\\Downloads\\5 to 1k tones.wav");

        try {
            WavFile wavFile = WavFile.openWavFile(file);
            wavFile.display();

            // Get the number of audio channels in the wav file
            int numChannels = wavFile.getNumChannels();
            sample_rate = wavFile.getSampleRate();

            // Create a buffer of 100 frames
            double[] buffer = new double[BUF_SIZE * numChannels];

            int framesRead;
            double min = Double.MAX_VALUE;
            int cur_sample = 0;
            int target_sample = (int)(TIME_OFFSET * sample_rate);


            while (cur_sample < target_sample) {

                // Read frames into buffer
                framesRead = wavFile.readFrames(buffer, BUF_SIZE);
                if (framesRead!=BUF_SIZE) {
                    System.out.println("EOF");
                    return;
                }

                cur_sample += BUF_SIZE / numChannels;

            }

            for (int i = 0; i < NUM_POINTS; i++) {
                s[i] = buffer[numChannels * i];

            }

        } catch (IOException e) {
            e.printStackTrace();
        } catch (WavFileException e) {
            e.printStackTrace();
        }


        signal.put(s);
    }

    static void do_something_with(DoublePointer result) {
        double[] r = new double[(int)result.capacity()];
        result.get(r);

        double bin_size = sample_rate / NUM_POINTS;

        for (int i = 0; i < NUM_POINTS / 2; i++) {
            double mag = sqrt(r[2 * i + REAL] * r[2 * i + REAL] +
                    r[2 * i + IMAG] * r[2 * i + IMAG]);

            System.out.println(bin_size * i + ": " + mag);
        }
    }


    /* Resume reading here */

    public static void main(String args[]) {
        Loader.load(fftw3.class);

        DoublePointer signal = new DoublePointer(NUM_POINTS);
        DoublePointer result = new DoublePointer(NUM_POINTS);

        fftw_plan plan = fftw_plan_dft_r2c_1d(NUM_POINTS, signal, result,
                (int)FFTW_PATIENT);

        acquire_from_somewhere(signal);
        fftw_execute(plan);
        do_something_with(result);

        fftw_destroy_plan(plan);
    }
}