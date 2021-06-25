
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
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.buttonLEFT = new System.Windows.Forms.Button();
            this.buttonRIGHT = new System.Windows.Forms.Button();
            this.buttonSTOP = new System.Windows.Forms.Button();
            this.buttonROOM = new System.Windows.Forms.Button();
            this.buttonBACK = new System.Windows.Forms.Button();
            this.buttonHOME = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // buttonGO
            // 
            this.buttonGO.Location = new System.Drawing.Point(604, 12);
            this.buttonGO.Name = "buttonGO";
            this.buttonGO.Size = new System.Drawing.Size(75, 50);
            this.buttonGO.TabIndex = 0;
            this.buttonGO.Text = "GO!";
            this.buttonGO.UseVisualStyleBackColor = true;
            this.buttonGO.Click += new System.EventHandler(this.buttonGO_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 125);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(667, 20);
            this.textBox1.TabIndex = 2;
            // 
            // buttonLEFT
            // 
            this.buttonLEFT.Location = new System.Drawing.Point(261, 69);
            this.buttonLEFT.Name = "buttonLEFT";
            this.buttonLEFT.Size = new System.Drawing.Size(75, 50);
            this.buttonLEFT.TabIndex = 3;
            this.buttonLEFT.Text = "L";
            this.buttonLEFT.UseVisualStyleBackColor = true;
            this.buttonLEFT.Click += new System.EventHandler(this.buttonLEFT_Click);
            // 
            // buttonRIGHT
            // 
            this.buttonRIGHT.Location = new System.Drawing.Point(342, 69);
            this.buttonRIGHT.Name = "buttonRIGHT";
            this.buttonRIGHT.Size = new System.Drawing.Size(75, 50);
            this.buttonRIGHT.TabIndex = 4;
            this.buttonRIGHT.Text = "R";
            this.buttonRIGHT.UseVisualStyleBackColor = true;
            this.buttonRIGHT.Click += new System.EventHandler(this.buttonRIGHT_Click);
            // 
            // buttonSTOP
            // 
            this.buttonSTOP.Location = new System.Drawing.Point(12, 12);
            this.buttonSTOP.Name = "buttonSTOP";
            this.buttonSTOP.Size = new System.Drawing.Size(75, 50);
            this.buttonSTOP.TabIndex = 5;
            this.buttonSTOP.Text = "STOP!";
            this.buttonSTOP.UseVisualStyleBackColor = true;
            this.buttonSTOP.Click += new System.EventHandler(this.buttonSTOP_Click);
            // 
            // buttonROOM
            // 
            this.buttonROOM.Location = new System.Drawing.Point(261, 12);
            this.buttonROOM.Name = "buttonROOM";
            this.buttonROOM.Size = new System.Drawing.Size(156, 50);
            this.buttonROOM.TabIndex = 6;
            this.buttonROOM.Text = "ENTER ROOM MODE!";
            this.buttonROOM.UseVisualStyleBackColor = true;
            this.buttonROOM.Click += new System.EventHandler(this.buttonROOM_Click);
            // 
            // buttonBACK
            // 
            this.buttonBACK.Location = new System.Drawing.Point(12, 69);
            this.buttonBACK.Name = "buttonBACK";
            this.buttonBACK.Size = new System.Drawing.Size(75, 50);
            this.buttonBACK.TabIndex = 7;
            this.buttonBACK.Text = "BACK!";
            this.buttonBACK.UseVisualStyleBackColor = true;
            this.buttonBACK.Click += new System.EventHandler(this.buttonBACK_Click);
            // 
            // buttonHOME
            // 
            this.buttonHOME.Location = new System.Drawing.Point(604, 69);
            this.buttonHOME.Name = "buttonHOME";
            this.buttonHOME.Size = new System.Drawing.Size(75, 50);
            this.buttonHOME.TabIndex = 8;
            this.buttonHOME.Text = "HOME!";
            this.buttonHOME.UseVisualStyleBackColor = true;
            this.buttonHOME.Click += new System.EventHandler(this.buttonHOME_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(691, 158);
            this.Controls.Add(this.buttonHOME);
            this.Controls.Add(this.buttonBACK);
            this.Controls.Add(this.buttonROOM);
            this.Controls.Add(this.buttonSTOP);
            this.Controls.Add(this.buttonRIGHT);
            this.Controls.Add(this.buttonLEFT);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.buttonGO);
            this.Name = "Form1";
            this.Text = "Form1";
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button buttonGO;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button buttonLEFT;
        private System.Windows.Forms.Button buttonRIGHT;
        private System.Windows.Forms.Button buttonSTOP;
        private System.Windows.Forms.Button buttonROOM;
        private System.Windows.Forms.Button buttonBACK;
        private System.Windows.Forms.Button buttonHOME;
    }
}

