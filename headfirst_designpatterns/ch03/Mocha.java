

public class Mocha extends CondimentDecorator
{
    Beverage beverage;

    public Mocha(Beverage beverage)
    {
        this.beverage = beverage;
        System.out.println("Mocha is created with size=" + beverage.getSize());
    }

    public String getDescription() // interface of CondimentDecorator
    {
        return beverage.getDescription() + ", MOCHA";
    }

    public int getSize() // interface of CondimentDecorator
    {
        return beverage.getSize();
    }

    public double cost()
    {
        if (beverage.getSize() == 1)
        {
            return 0.2 + beverage.cost();
        }
        else if (beverage.getSize() == 2)
        {
            return 0.3 + beverage.cost();
        }
        else if (beverage.getSize() == 3)
        {
            return 0.4 + beverage.cost();
        }
        else
        {
            return 0;
        }
    }
}
