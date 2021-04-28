using System;
using System.Windows.Forms;

namespace vis1
{
    public partial class SingleShotSetVals : Form
    {
        public float SingleShotTrigger;
        public int SingleShotChannel;

        public SingleShotSetVals(float ssTin, int ssCin)
        {
            InitializeComponent();
            textBox1.Text = $@"{ssTin}";
            numericUpDown1.Text = $@"{ssCin + 1}";
            SingleShotTrigger = ssTin;
            SingleShotChannel = ssCin;
        }

        private void OK_Click(object sender, EventArgs e)
        {
            SingleShotChannel = numericUpDown1.DecimalPlaces - 1;
            SingleShotTrigger = float.Parse(textBox1.Text);
            Close();
        }

        private void Cancel_Click(object sender, EventArgs e)
        {
            Close();
        }
    }
}
