using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Windows_Form
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)
        {
            ((Button)sender).Text = "我要生成新按钮了呢";
            Button newButton = new Button();
            newButton.Text = "我是新按钮";
            newButton.Location = new Point(0, 10);
            newButton.Click += new EventHandler(btNew_Click);
            this.Controls.Add(newButton);
        }

        private void btNew_Click(object sender, EventArgs e)
        {
            ((Button)sender).Text = "我被点击了";
        }


    }
}
