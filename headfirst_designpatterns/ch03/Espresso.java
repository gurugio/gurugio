

public class Espresso extends Beverage
{
    public Espresso(int size)
    {
        description = "ESPRESSO";
        setSize(size);
    }

    public double cost()
    {
        return 1.99;
    }
}
