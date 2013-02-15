

public class Soy extends CondimentDecorator
{
    Beverage beverage;

    public Soy(Beverage beverage)
    {
        this.beverage = beverage;
    }

    public String getDescription()
    {
        return beverage.getDescription() + ", Soy";
    }

    public int getSize() // interface of CondimentDecorator
    {
        return beverage.getSize();
    }


    public double cost()
    {
        if (beverage.getSize() == 1)        
            return 0.15 + beverage.cost();
        else if (beverage.getSize() == 2)
            return 0.20 + beverage.cost();
        else
            return 0.30 + beverage.cost();
    }
}

