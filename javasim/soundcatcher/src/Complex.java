public class Complex
{
    double real;
    double imag;

    public Complex(double real, double imag) {
        this.real = real;
        this.imag = imag;
    }
    public Complex scale(double in)
    {
        return new Complex(real * in, imag * in);
    }
    public Complex conjugate()
    {
        return new Complex(real, -imag);
    }

    public Complex plus(Complex other)
    {
        return new Complex(real + other.real, imag + other.imag);
    }

    public Complex minus(Complex other)
    {
        return new Complex(real - other.real, imag - other.imag);
    }

    public Complex times(Complex other)
    {
        double new_real = real * other.real - imag * other.imag;
        double new_imag = imag * other.real + real * other.imag;
        return new Complex(new_real, new_imag);
    }

    public double abs()
    {
        return Math.sqrt(Math.pow(real,2) + Math.pow(imag,2));
    }

    public String toString() {
        return "(" + real+","+imag+")";
    }

    public static Complex[] fromReal(double reals[], int len)
    {
        Complex c[] = new Complex[len];

        for (int i = 0; i < len; i++) {
            c[i] = new Complex(reals[i], 0);
        }

        return c;
    }

}