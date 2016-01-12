using System;

class BaseClass { }

class ClassA : BaseClass { }

interface InterfaceA { }

struct StructA : InterfaceA { }

class ClassB : InterfaceA { }

class Learn_console
{
    public static void Main(string[] args)
    {
        #region 测试is的用法
        BaseClass base_ = new BaseClass();
        ClassA a = new ClassA();
        ClassB b = new ClassB();
        StructA s;
        int i = 1;
        Console.WriteLine("base_ is Object {0}", base_ is Object);
        Console.WriteLine("base_ is BaseClass {0}", base_ is BaseClass);
        Console.WriteLine("a is Object {0}", a is Object);
        Console.WriteLine("a is BaseClass {0}", a is BaseClass);
        Console.WriteLine("a is ClassA {0}", a is ClassA);
        Console.WriteLine("b is Object {0}", b is Object);
        Console.WriteLine("b is Interface {0}", b is InterfaceA);
        Console.WriteLine("b is ClassB {0}", b is ClassB);
        Console.WriteLine("s is Object {0}", s is Object);
        Console.WriteLine("s is BaseClass {0}", s is BaseClass);
        Console.WriteLine("s is Imterface {0}", s is InterfaceA);
        Console.WriteLine("i is Object {0}", i is Object);
        #endregion

        #region 测试typeof的用法        
        Console.WriteLine("base_ is Object {0}", base_.GetType() == typeof(Object));
        Console.WriteLine("base_ is BaseClass {0}", base_.GetType() == typeof(BaseClass));
        Console.WriteLine("a is Object {0}", a.GetType() == typeof(Object));
        Console.WriteLine("a is BaseClass {0}", a.GetType() == typeof(BaseClass));
        Console.WriteLine("a is ClassA {0}", a.GetType() == typeof(ClassA));
        Console.WriteLine("b is Object {0}", b.GetType() == typeof(Object));
        Console.WriteLine("b is Interface {0}", b.GetType() == typeof(InterfaceA));
        Console.WriteLine("b is ClassB {0}", b.GetType() == typeof(ClassB));
        Console.WriteLine("s is Object {0}", s.GetType() == typeof(Object));
        Console.WriteLine("s is BaseClass {0}", s.GetType() == typeof(BaseClass));
        Console.WriteLine("s is Imterface {0}", s.GetType() == typeof(InterfaceA));
        Console.WriteLine("i is Object {0}", i.GetType() == typeof(Object));
        #endregion
    
    
    }

    
}

