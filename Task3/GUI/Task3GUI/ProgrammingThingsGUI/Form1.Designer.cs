
namespace ProgrammingThingsGUI
{
    partial class Form1
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
            this.buttonGO = new System.Windows.Forms.Button();
            this.buttonCONTINUE = new System.Windows.Forms.Button();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.buttonLEFT = new System.Windows.Forms.Button();
            this.buttonRIGHT = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonGO
            // 
            this.buttonGO.Location = new System.Drawing.Point(253, 13);
            this.buttonGO.Name = "buttonGO";
            this.buttonGO.Size = new System.Drawing.Size(75, 50);
            this.buttonGO.TabIndex = 0;
            this.buttonGO.Text = "GO!";
            this.buttonGO.UseVisualStyleBackColor = true;
            this.buttonGO.Click += new System.EventHandler(this.buttonGO_Click);
            // 
            // buttonCONTINUE
            // 
            this.buttonCONTINUE.Location = new System.Drawing.Point(262, 125);
            this.buttonCONTINUE.Name = "buttonCONTINUE";
            this.buttonCONTINUE.Size = new System.Drawing.Size(56, 36);
            this.buttonCONTINUE.TabIndex = 1;
            this.buttonCONTINUE.Text = "C";
            this.buttonCONTINUE.UseVisualStyleBackColor = true;
            this.buttonCONTINUE.Click += new System.EventHandler(this.buttonCONTINUE_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 167);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(540, 20);
            this.textBox1.TabIndex = 2;
            // 
            // buttonLEFT
            // 
            this.buttonLEFT.Location = new System.Drawing.Point(211, 69);
            this.buttonLEFT.Name = "buttonLEFT";
            this.buttonLEFT.Size = new System.Drawing.Size(75, 50);
            this.buttonLEFT.TabIndex = 3;
            this.buttonLEFT.Text = "L";
            this.buttonLEFT.UseVisualStyleBackColor = true;
            this.buttonLEFT.Click += new System.EventHandler(this.buttonLEFT_Click);
            // 
            // buttonRIGHT
            // 
            this.buttonRIGHT.Location = new System.Drawing.Point(292, 69);
            this.buttonRIGHT.Name = "buttonRIGHT";
            this.buttonRIGHT.Size = new System.Drawing.Size(75, 50);
            this.buttonRIGHT.TabIndex = 4;
            this.buttonRIGHT.Text = "R";
            this.buttonRIGHT.UseVisualStyleBackColor = true;
            this.buttonRIGHT.Click += new System.EventHandler(this.buttonRIGHT_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(564, 206);
            this.Controls.Add(this.buttonRIGHT);
            this.Controls.Add(this.buttonLEFT);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.buttonCONTINUE);
            this.Controls.Add(this.buttonGO);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonGO;
        private System.Windows.Forms.Button buttonCONTINUE;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button buttonLEFT;
        private System.Windows.Forms.Button buttonRIGHT;
    }
}

