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
    #region 属性赋值的类型转换器
    //指定类型转换器
    [TypeConverterAttribute(typeof(StringToHuman))]
    public class Human
    {
        public string idName {get; set;}
        public Human child { get; set; }

    }
    #endregion

    #region 应用x:Type标签，动态初始化窗口
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

            ////应用x:Key
            //Human h = (Human)this.FindResource("human");
            //MessageBox.Show(h.child.name);

            //新建并弹出窗口
            this.Content = "弹出新窗口";
            Window newWindow = Activator.CreateInstance(windowGenerated) as Window;
            if (newWindow != null)
            {
                newWindow.ShowDialog();
            }
        }
    }
    #endregion

    #region 校验类，继承ValidationRule
    public class SliderValidationRule : ValidationRule
    {
        public override ValidationResult Validate(object value, System.Globalization.CultureInfo cultureInfo)
        {
            double d;
            //验证是数字而且在0-100
            if (double.TryParse(value.ToString(), out d))
            {
                if (d >= 0 && d <= 100) 
                    return new ValidationResult(true, null);
            } 
            
            return new ValidationResult(false, "范围错误偶");
        }
    }
    #endregion
   
    /// <summary>
    /// MainWindow.xaml 的交互逻辑
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            #region 校验器实例【默认不检查target赋值】！！！！注意校验发生在target一方
            SliderValidationRule sliderValidationRule = new SliderValidationRule();
            sliderValidationRule.ValidatesOnTargetUpdated = true;//双向检查

            Binding binding = new Binding("Value") { Source = sliderShow };
            binding.ValidationRules.Add(sliderValidationRule);
            binding.NotifyOnValidationError = true;//还需要添加事件偶
            binding.UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged;//即时更新
            textBoxShow.SetBinding(TextBox.TextProperty, binding);

            textBoxShow.AddHandler(Validation.ErrorEvent, new RoutedEventHandler(validationErrorHandler));//添加事件处理
            #endregion
        }

        void validationErrorHandler(object sender, RoutedEventArgs e)
        {
            if (Validation.GetErrors(textBoxShow).Count > 0)
                textBoxShow.ToolTip = Validation.GetErrors(textBoxShow)[0].ErrorContent.ToString();
        }
         
        
    }
}
