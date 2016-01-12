using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace learn_wpf_xaml
{
    public class StringToHuman : TypeConverter
    {
        public override object ConvertFrom(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value)
        {
            //类型转换，将string转化为Human对象
            if (value is string)
            {
                Human h = new Human();
                h.name = value as string;
                return (Object)h;
            }
            return base.ConvertFrom(context, culture, value);
        }
    }
}
