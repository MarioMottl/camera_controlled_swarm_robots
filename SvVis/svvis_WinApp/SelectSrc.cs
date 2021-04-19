using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace vis1
{
    public partial class SelectSrc : Form
    {
        public bool useSocket;
        public bool useCOM;
        public SelectSrc()
        {
            AcceptButton = buttonCOM;
            InitializeComponent();
            ThemeHandler.ApplyTheme(this);
        }

        private void button1_Click(object sender, EventArgs e) // button1_Click because I forgot to change the name to buttonCOM before generating this method
        {
            this.useSocket = false;
            this.useCOM = true;
            this.Close();
        }

        private void buttonIP_Click(object sender, EventArgs e)
        {
            this.useSocket = true;
            this.useCOM = false;
            this.Close();
        }
    }
}
