
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
            this.buttonUP = new System.Windows.Forms.Button();
            this.buttonRIGHT = new System.Windows.Forms.Button();
            this.buttonDOWN = new System.Windows.Forms.Button();
            this.buttonLEFT = new System.Windows.Forms.Button();
            this.buttonSTOP = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonUP
            // 
            this.buttonUP.Location = new System.Drawing.Point(93, 12);
            this.buttonUP.Name = "buttonUP";
            this.buttonUP.Size = new System.Drawing.Size(75, 23);
            this.buttonUP.TabIndex = 0;
            this.buttonUP.Text = "↑";
            this.buttonUP.UseVisualStyleBackColor = true;
            this.buttonUP.Click += new System.EventHandler(this.buttonUP_Click);
            // 
            // buttonRIGHT
            // 
            this.buttonRIGHT.Location = new System.Drawing.Point(174, 41);
            this.buttonRIGHT.Name = "buttonRIGHT";
            this.buttonRIGHT.Size = new System.Drawing.Size(75, 23);
            this.buttonRIGHT.TabIndex = 1;
            this.buttonRIGHT.Text = "→";
            this.buttonRIGHT.UseVisualStyleBackColor = true;
            this.buttonRIGHT.Click += new System.EventHandler(this.buttonRIGHT_Click);
            // 
            // buttonDOWN
            // 
            this.buttonDOWN.Location = new System.Drawing.Point(93, 41);
            this.buttonDOWN.Name = "buttonDOWN";
            this.buttonDOWN.Size = new System.Drawing.Size(75, 23);
            this.buttonDOWN.TabIndex = 2;
            this.buttonDOWN.Text = "↓";
            this.buttonDOWN.UseVisualStyleBackColor = true;
            this.buttonDOWN.Click += new System.EventHandler(this.buttonDOWN_Click);
            // 
            // buttonLEFT
            // 
            this.buttonLEFT.Location = new System.Drawing.Point(12, 41);
            this.buttonLEFT.Name = "buttonLEFT";
            this.buttonLEFT.Size = new System.Drawing.Size(75, 23);
            this.buttonLEFT.TabIndex = 3;
            this.buttonLEFT.Text = "←";
            this.buttonLEFT.UseVisualStyleBackColor = true;
            this.buttonLEFT.Click += new System.EventHandler(this.buttonLEFT_Click);
            // 
            // buttonSTOP
            // 
            this.buttonSTOP.Location = new System.Drawing.Point(12, 73);
            this.buttonSTOP.Name = "buttonSTOP";
            this.buttonSTOP.Size = new System.Drawing.Size(237, 23);
            this.buttonSTOP.TabIndex = 4;
            this.buttonSTOP.Text = "STOP";
            this.buttonSTOP.UseVisualStyleBackColor = true;
            this.buttonSTOP.Click += new System.EventHandler(this.buttonSTOP_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(260, 113);
            this.Controls.Add(this.buttonSTOP);
            this.Controls.Add(this.buttonLEFT);
            this.Controls.Add(this.buttonDOWN);
            this.Controls.Add(this.buttonRIGHT);
            this.Controls.Add(this.buttonUP);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button buttonUP;
        private System.Windows.Forms.Button buttonRIGHT;
        private System.Windows.Forms.Button buttonDOWN;
        private System.Windows.Forms.Button buttonLEFT;
        private System.Windows.Forms.Button buttonSTOP;
    }
}

