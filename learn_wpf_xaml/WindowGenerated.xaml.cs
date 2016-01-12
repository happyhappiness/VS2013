using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace learn_wpf_xaml
{

    public class Employee
    {
        public int id{get; set;}
        public string name{ get; set;}
    }
    /// <summary>
    /// WindowGenerated.xaml 的交互逻辑
    /// </summary>
    public partial class WindowGenerated : Window
    {
        //应用x:Static
        public static string title = "我的窗口";
        public List<Employee> myEmployee = new List<Employee>{ 
            new Employee { id = 1, name = "小李子" },
            new Employee { id = 2, name = "小刚子" },
            new Employee { id = 3, name = "小珠子" }
        };//数组初始化语法
       
        public WindowGenerated()
        {
            InitializeComponent();

            ////填充myEmployee并显示在ListBox
            //myEmployee.Add(new Employee { id = 1, name = "小李子" });
            //myEmployee.Add(new Employee { id = 2, name = "小刚子" });
            //myEmployee.Add(new Employee { id = 3, name = "小珠子" });//对象初始化语法
            myListBox.DisplayMemberPath = "name";
            myListBox.SelectedValuePath = "id";
            myListBox.ItemsSource = myEmployee;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ((Button)sender).Content = "我已经被点击了";
            DependencyObject level1 = VisualTreeHelper.GetParent(btClick);
            DependencyObject level2 = VisualTreeHelper.GetParent(level1);
            DependencyObject level3 = VisualTreeHelper.GetParent(level2);
            MessageBox.Show("level1 is " + level1.GetType().ToString()+ "\n" +
                "level2 is " + level2.GetType().ToString() + "\n" +
                "level3 is " + level3.GetType().ToString());
        }
    }
}
