
namespace vis1
{
    partial class SelectSrc
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.buttonCOM = new System.Windows.Forms.Button();
            this.buttonIP = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // buttonCOM
            // 
            this.buttonCOM.Location = new System.Drawing.Point(12, 47);
            this.buttonCOM.Name = "buttonCOM";
            this.buttonCOM.Size = new System.Drawing.Size(75, 34);
            this.buttonCOM.TabIndex = 0;
            this.buttonCOM.Text = "COM Port";
            this.buttonCOM.UseVisualStyleBackColor = true;
            this.buttonCOM.Click += new System.EventHandler(this.button1_Click);
            // 
            // buttonIP
            // 
            this.buttonIP.Location = new System.Drawing.Point(93, 47);
            this.buttonIP.Name = "buttonIP";
            this.buttonIP.Size = new System.Drawing.Size(75, 34);
            this.buttonIP.TabIndex = 1;
            this.buttonIP.Text = "TCP / IP connection";
            this.buttonIP.UseVisualStyleBackColor = true;
            this.buttonIP.Click += new System.EventHandler(this.buttonIP_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 9);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(150, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "How do you want to connect?";
            // 
            // SelectSrc
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(175, 93);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.buttonIP);
            this.Controls.Add(this.buttonCOM);
            this.Name = "SelectSrc";
            this.Text = "Select Data Source";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonCOM;
        private System.Windows.Forms.Button buttonIP;
        private System.Windows.Forms.Label label1;
    }
}