

public abstract class Beverage
{
    String description = "No title";
    int size;

    public String getDescription()
    {
        return description;
    }

    public void setSize(int s)
    {
        size = s;
    }

    public int getSize()
    {
        return size;
    }

    public abstract double cost();

}

