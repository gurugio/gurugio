

public class SteamMilk extends CondimentDecorator
{
    Beverage beverage;

    public SteamMilk(Beverage beverage)
    {
        this.beverage = beverage;
    }

    public String getDescription()
    {
        return beverage.getDescription() + ", STEAM MILK";
    }

    public int getSize() // interface of CondimentDecorator
    {
        return beverage.getSize();
    }

    public double cost()
    {
        return 0.1 + beverage.cost();
    }
}
