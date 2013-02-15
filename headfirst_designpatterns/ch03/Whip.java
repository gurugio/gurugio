

public class Whip extends CondimentDecorator
{
    Beverage beverage;

    public Whip(Beverage beverage)
    {
        this.beverage = beverage;
    }

    public String getDescription()
    {
        return beverage.getDescription() + ", WHIP";
    }

    public int getSize() // interface of CondimentDecorator
    {
        return beverage.getSize();
    }

    public double cost()
    {
        if (beverage.getSize() == 1)
        {
            return 0.1 + beverage.cost();
        }
        else if (beverage.getSize() == 2)
            return 0.2 + beverage.cost();
        else
            return 0.3 + beverage.cost();
    }
}
