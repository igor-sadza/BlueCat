using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Data.SqlClient;

namespace WinFormsApp2
{

    public partial class Form1 : Form
    {
        public string client_mail;

        public Form1()
        {
            InitializeComponent();

            textBox2.ForeColor = SystemColors.GrayText;
            textBox2.Text = "Provide client email to unlock generate button";
            this.textBox2.Leave += new System.EventHandler(this.textBox2_Leave);
            this.textBox2.Enter += new System.EventHandler(this.textBox2_Enter);
            generate.Enabled = false;
        }

        private void generate_Click(object sender, EventArgs e)
        {
            bioexploer_textBox.Text = "";
            Random rnd = new Random();
            for (int i = 0; i < 25; i++)
            {
                if (i % 5 == 0 && i != 0)
                {
                    bioexploer_textBox.Text += '-';
                }
                bioexploer_textBox.Text += (char)rnd.Next(65, 90);
            }
            string connetionString;
            SqlConnection cnn;
            connetionString = @"server=tcp:SERVER-NAME,SERVER-PORT;Initial Catalog=IS-DEV;User ID=USER;Password=PASSWORD";
            cnn = new SqlConnection(connetionString);
            cnn.Open();

            string query = "INSERT INTO dbo.ISBioExplorer";
            query += " VALUES ('" + DateTime.Now.ToString() + "','" + bioexploer_textBox.Text + "',0,'"+ client_mail.ToString() + "',0,0)";

            SqlCommand myCommand = new SqlCommand(query, cnn);
            myCommand.ExecuteNonQuery();

            isflashplayer_textbox.Text = "";
            rnd = new Random();
            for (int i = 0; i < 25; i++)
            {
                if (i % 5 == 0 && i != 0)
                {
                    isflashplayer_textbox.Text += '-';
                }
                isflashplayer_textbox.Text += (char)rnd.Next(65, 90);
            }

            cnn = new SqlConnection(connetionString);
            cnn.Open();

            query = "INSERT INTO dbo.ISFlashPlayer";
            query += " VALUES ('" + isflashplayer_textbox.Text + "', 0,0,0,'" + DateTime.Now.ToString() + "')";

            myCommand = new SqlCommand(query, cnn);
            myCommand.ExecuteNonQuery();

            cnn.Close();
        }

        private void textBox2_TextChanged(object sender, EventArgs e)
        {
            client_mail = textBox2.Text;
            if (client_mail == "")
            {
                generate.Enabled = false;

            }
            else if(client_mail != "Provide client email to unlock generate button")
            {
                if(IsValidEmail(client_mail))
                {
                    generate.Enabled = true;
                }
           
            }
        }

        public void textBox2_Leave(object sender, EventArgs e)
        {
            if (textBox2.Text.Length == 0)
            {
                //generate.Enabled = false;
                textBox2.Text = "Provide client email to unlock generate button";
                textBox2.ForeColor = SystemColors.GrayText;
                client_mail = "";
            }
        }

        private void textBox2_Enter(object sender, EventArgs e)
        {
            if (textBox2.Text == "Provide client email to unlock generate button")
            {
               // generate.Enabled = false;
                textBox2.Text = "";
                textBox2.ForeColor = SystemColors.WindowText;
                client_mail = "";
            }
        }

        private bool IsValidEmail(string email)
        {
            try
            {
                var addr = new System.Net.Mail.MailAddress(email);
                return addr.Address == email;
            }
            catch
            {
                return false;
            }
        }

    }
}
