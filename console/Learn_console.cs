using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Diagnostics;

namespace console
{

    class Learn_console
    {
        
        static void Main(string[] args)
        {
            string eType;
            while(true)
            {
                Console.WriteLine("请输入想要抛出的异常类型");
                eType = Console.ReadLine();
                try
                {
                    ThrowException(eType);
                }
                catch (IndexOutOfRangeException e)
                {
                    Console.WriteLine(e.ToString());
                    Console.WriteLine("catch index out of range");
                }
                catch(Exception e)
                {
                    Console.WriteLine(e.ToString());
                    Console.WriteLine("catch");
                }
                finally
                {
                    Console.WriteLine("到finally了偶");
                }
            }

        }

    private static void ThrowException(string eType)
    {
        switch (eType)
        { 
            case "none":
                //donothing
                break;
            case "index":
                throw new IndexOutOfRangeException();
                break;
            case "nested index":
                try
                {
                    throw new IndexOutOfRangeException();
                }
                catch (Exception e)
                {
                    Console.WriteLine("nested index catch");
                    e.ToString();
                    throw;
                }
                finally
                {
                    Console.WriteLine("nested index finally");
                }
                break;
            case "simple":
                throw new SystemException();
                break;
        }
    }
    }
}

