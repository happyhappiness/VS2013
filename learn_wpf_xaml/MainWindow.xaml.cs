using System;
using System.Collections.Generic;
using System.ComponentModel;
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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace learn_wpf_xaml
{
    //指定类型转换器
    [TypeConverterAttribute(typeof(StringToHuman))]
    public class Human
    {
        public string name {get; set;}
        public Human child { get; set; }

    }

    /// <summary>
    /// 自定义MyButton类，按钮弹出新window
    /// </summary>
    public class MyButton : Button
    {
        //应用x:Type
        public Type windowGenerated { set; get; }

        protected override void OnClick()
        {
            base.OnClick();

            //应用x:Key
            Human h = (Human)this.FindResource("human");
            MessageBox.Show(h.child.name);

            //新建并弹出窗口
            this.Content = "弹出新窗口";
            Window newWindow = Activator.CreateInstance(windowGenerated) as Window;
            if (newWindow != null)
            {
                newWindow.ShowDialog();
            }
        }
    }


    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        
    }
}
