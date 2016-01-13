using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
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
using System.Xml.Linq;

namespace learn_wpf_xaml
{
    #region Employee类，实现了INotifyPropertyChanged接口
    public class Employee : INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public int Id{get; set;}
        private string name;
        public string Name
        {
            get 
            {
                return name;
            }

            set 
            {
                name = value;
                if (PropertyChanged != null)
                {
                    PropertyChanged.Invoke(this, new PropertyChangedEventArgs("Name"));
                }
            }
        }
        public int Age { get; set; }
    }
    #endregion

    #region AgeToImage类转换类，实现IValueConverter接口
    public class AgeToImage : IValueConverter
    {     
        //age to imageUri
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            if (int.Parse(value.ToString()) < 30)
                return @"\Image\2.jpg";
            else
                return @"\Image\3.jpg";
        }

        //OneWay
        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
 	        throw new NotImplementedException();
        }
    }
    #endregion
    
    /// <summary>
    /// WindowGenerated.xaml 的交互逻辑
    /// </summary>
    public partial class WindowGenerated : Window
    {
        //应用x:Static
        public static string title = "我的窗口";
        public List<Employee> myEmployee;
       
        public WindowGenerated()
        {
            InitializeComponent();

            #region linq访问xml
            //XDocument xdc = XDocument.Load(@"G:\c++\c#\console\employee.xml");
            //listViewEmployee.ItemsSource = from element in xdc.Descendants("employee")
            //                               where element.Attribute("Name").Value.StartsWith("小")
            //                               select new Employee()
            //                               {
            //                                   Id = int.Parse(element.Attribute("Id").Value),
            //                                   Name = element.Attribute("Name").ToString(),
            //                                   Age = int.Parse(element.Attribute("Age").Value)
            //                               };
            #endregion

            #region linq访问DataTable
            //DataTable dt = loadDataTable();
            //listViewEmployee.ItemsSource = from row in dt.Rows.Cast<DataRow>()
            //                               where row["Name"].ToString().StartsWith("李")
            //                               select new Employee()
            //                               {
            //                                   Id = Convert.ToInt32(row["Id"]),
            //                                   Name = Convert.ToString(row["Name"]),
            //                                   Age = Convert.ToInt32(row["Age"])
            //                               };
            #endregion

            XmlDataProvider xdp = new XmlDataProvider();
            xdp.Source = new Uri(@"G:\c++\c#\console\employee.xml");
            xdp.XPath = "/employeeList/employee";
            listViewEmployee.DataContext = xdp;
            listViewEmployee.SetBinding(ListView.ItemsSourceProperty, new Binding());

            //将TextBox控件绑定到BindingChild
            Binding bindingId = new Binding() { Path = new PropertyPath("SelectedItem.Id"), Source = listViewEmployee };
            textBoxId.SetBinding(TextBox.TextProperty, bindingId);
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ((Button)sender).Content = "我已经被点击了";
            DependencyObject level1 = VisualTreeHelper.GetParent(btClick);
            DependencyObject level2 = VisualTreeHelper.GetParent(level1);
            DependencyObject level3 = VisualTreeHelper.GetParent(level2);
            MessageBox.Show("level1 is " + level1.GetType().ToString() + "\n" +
                "level2 is " + level2.GetType().ToString() + "\n" +
                "level3 is " + level3.GetType().ToString());//获取可视控件树的父类

        }

        #region 生成DataTable
        /// <summary>
        /// 生成并填充DataTable
        /// </summary>
        /// <returns></returns>
        private DataTable loadDataTable()
        {

            DataTable dt = new DataTable();
            dt.Columns.Add("Id", typeof(int));
            dt.Columns.Add("Name", typeof(string));
            dt.Columns.Add("Age", typeof(int));
            DataRow dr = dt.NewRow();
            dr["Id"] = 1;
            dr["Name"] = "李婷";
            dr["Age"] = 12;
            dt.Rows.Add(dr);
            dr = dt.NewRow();
            dr["Id"] = 2;
            dr["Name"] = "周云";
            dr["Age"] = 21;
            dt.Rows.Add(dr);
            dr = dt.NewRow();
            dr["Id"] = 3;
            dr["Name"] = "张敏";
            dr["Age"] = 19;
            dt.Rows.Add(dr);
            
            return dt;
        }
        #endregion
    }
}
