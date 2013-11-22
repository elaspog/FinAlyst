using System;
using System.IO;
using System.Net;
using System.Web;
using System.Text;
using System.Runtime.Serialization;
using System.Runtime.Serialization.Json;

class FinAlyst
{

    [DataContract]
    public class LoginResponse
    {
        [DataMember(Name = "success")]
        public bool Success { get; set; }
        [DataMember(Name = "status")]
        public int Status { get; set; }
        [DataMember(Name = "sessionid")]
        public string SessionID { get; set; }
    }

    [DataContract]
    public class Category
    {
        [DataMember(Name = "id")]
        public int Id { get; set; }
        [DataMember(Name = "create")]
        public DateTime Create { get; set; }
        [DataMember(Name = "modify")]
        public DateTime Modify { get; set; }
        [DataMember(Name = "name")]
        public string Name { get; set; }
        [DataMember(Name = "description")]
        public string Description { get; set; }
    }

    [DataContract]
    public class CategoriesResponse
    {
        [DataMember(Name = "success")]
        public bool Success { get; set; }
        [DataMember(Name = "status")]
        public int Status { get; set; }
        [DataMember(Name = "data")]
        public Category[] Categories { get; set; }
    }

    static Stream GenerateStreamFromString(string s)
    {
        MemoryStream stream = new MemoryStream();
        StreamWriter writer = new StreamWriter(stream);
        writer.Write(s);
        writer.Flush();
        stream.Position = 0;
        return stream;
    }

    static string Login(string hostname)
    {
        Stream dataStream;
        WebRequest request;
        string datastr;
        UTF8Encoding encoding = new UTF8Encoding();
        
        Console.WriteLine("### Login request ###");
        Console.Write("username: ");
        string username = Console.ReadLine();
        Console.Write("password: ");
        string password = Console.ReadLine();
        request = WebRequest.Create("http://"+hostname+"/fcgi-bin/finance?q=webservice/login");
        request.Method = "POST";
        request.ContentType = "application/x-www-form-urlencoded";
        datastr = "username="+username+"&password="+password;
        using (dataStream = request.GetRequestStream())
        {
            string urlencoded = Uri.EscapeUriString(datastr);
            byte[] data = encoding.GetBytes(urlencoded);
            dataStream.Write(data, 0, data.Length);
        }
        WebResponse response = request.GetResponse();
        Console.WriteLine("Response:");
        using (dataStream = response.GetResponseStream ())
        {
            StreamReader reader = new StreamReader (dataStream);
            string responseFromServer = reader.ReadToEnd ();
            Console.WriteLine(responseFromServer);
            DataContractJsonSerializer serializer =
                new DataContractJsonSerializer(typeof(LoginResponse));
            using (Stream s = GenerateStreamFromString(responseFromServer))
            {
                LoginResponse loginresponse = (LoginResponse)serializer.ReadObject(s);
                string sessionid = loginresponse.SessionID;
                Console.WriteLine("sessionid: {0}", sessionid);
                return sessionid;
            }
        }
    }

    static Category[] ListCategories(string hostname, string sessionid)
    {
        Stream dataStream;
        WebRequest request;

        Console.WriteLine("### List categories ###");
        string url = "http://"+hostname+"/fcgi-bin/finance?q=webservice/categories&sessionid="+sessionid;
        request = WebRequest.Create(Uri.EscapeUriString(url));
        request.Method = "GET";
        WebResponse response = request.GetResponse();
        Console.WriteLine("Response:");
        DataContractJsonSerializer serializer =
                new DataContractJsonSerializer(typeof(CategoriesResponse));
        using (dataStream = response.GetResponseStream ())
        {
            CategoriesResponse resp = (CategoriesResponse)serializer.ReadObject(dataStream);
            //if (!resp.Success) return null;
            return resp.Categories;
        }
    }

    static void Logout(string hostname, string sessionid)
    {
        Stream dataStream;
        WebRequest request;
        string datastr;
        UTF8Encoding encoding = new UTF8Encoding();

        Console.WriteLine("### Logout ###");
        request = WebRequest.Create("http://"+hostname+"/fcgi-bin/finance?q=webservice/logout");
        request.Method = "POST";
        request.ContentType = "application/x-www-form-urlencoded";
        datastr = "sessionid="+sessionid;
        using (dataStream = request.GetRequestStream())
        {
            string urlencoded = Uri.EscapeUriString(datastr);
            byte[] data = encoding.GetBytes(urlencoded);
            dataStream.Write(data, 0, data.Length);
        }
        WebResponse response = request.GetResponse();
        Console.WriteLine("Response:");
        using (dataStream = response.GetResponseStream ())
        {
            StreamReader reader = new StreamReader (dataStream);
            string responseFromServer = reader.ReadToEnd ();
            Console.WriteLine(responseFromServer);
        }
    }

    static void Main()
    {
        Console.Write("Host: ");
        string hostname = Console.ReadLine();
        string sessionid = Login(hostname);
        Console.WriteLine();
        int error = 0;
        for (int i = 0; i < 100; ++i)
        {
            Category[] categories = ListCategories(hostname, sessionid); 
            if (categories == null)
            {
                Console.WriteLine("Got categories: ERROR");
                ++error;
            } else
                Console.WriteLine("Got categories: {0}", categories.Length);
        }
        Logout(hostname, sessionid);
        Console.WriteLine("Request error count: {0}", error);
    }

}
