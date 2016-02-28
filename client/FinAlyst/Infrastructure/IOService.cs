using FinAlyst.Infrastructure.Helpers;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using System.Xml.Serialization;

namespace FinAlyst.Infrastructure
{
    public class IOService
    {
        public IOService()
        { 
        }

        public ConfigurationDescriptor GetConnections(string fileName, ConfigurationDescriptor configurationDescriptor)
        {
            XmlSerializer serializer = new XmlSerializer(typeof(ConfigurationDescriptor));
            ConfigurationDescriptor config = null;

            try
            {
                StreamReader reader = new StreamReader(fileName);
                config = (ConfigurationDescriptor)serializer.Deserialize(reader);
            }
            catch (FileNotFoundException)
            {
                StreamWriter writer = new StreamWriter(fileName);
                serializer.Serialize(writer, configurationDescriptor);
                writer.Dispose();
                StreamReader reader2 = new StreamReader(fileName);
                config = (ConfigurationDescriptor)serializer.Deserialize(reader2);
            }
            catch (Exception)
            {
            }
            return config;
        }


        public void WriteConfiguration(string fileName, ConfigurationDescriptor configurationDescriptor)
        {         
            XmlSerializer serializer = new XmlSerializer(typeof(ConfigurationDescriptor));
            try
            {
                StreamWriter writer = new StreamWriter(fileName);
                serializer.Serialize(writer, configurationDescriptor);
                writer.Dispose();
            }
            catch (Exception)
            {
            }
        }



        //public System.IO.FileStream GetFileStream(string SelectedPath)
        //{
        //    return File.OpenRead(SelectedPath);
        //}



        //public ConfigurationDescriptor ReadConfiguratuion(System.IO.Stream XMLInputstream)
        //{
        //    XmlSerializer serializer = new XmlSerializer(typeof(ConfigurationDescriptor));
        //    ConfigurationDescriptor configurationDescriptor = null;
        //    try
        //    {
        //        using (XmlReader reader = XmlReader.Create(XMLInputstream))
        //        {
        //            configurationDescriptor = (ConfigurationDescriptor)serializer.Deserialize(reader);
        //        }
        //    }
        //    catch (Exception)
        //    {
        //        return null;
        //    }
        //    return configurationDescriptor;
        //}


        //public void WriteConfiguration(string fileName, ConfigurationDescriptor configurationDescriptor)
        //{
        //    XmlSerializer serializer = new XmlSerializer(typeof(ConfigurationDescriptor));
        //    try
        //    {

        //        StreamReader reader = new StreamReader(fileName);
        //        ConfigurationDescriptor config = (ConfigurationDescriptor)serializer.Deserialize(reader);

        //        Console.WriteLine("done");
        //    }
        //    catch (Exception e)
        //    {
        //        Console.WriteLine(e.ToString());
        //    }
        //}








        //string RootElementName = null;
        //System.Xml.Linq.XDocument doc;



        //StreamWriter writer = new StreamWriter(fileName);
        //serializer.Serialize(writer, configurationDescriptor);
        //writer.Dispose();


        //GetFileStream(fileName);
        //doc = System.Xml.Linq.XDocument.Load(fileName);
        //RootElementName = doc.Root.Name.LocalName;


        //doc = new XDocument(
        //                    new XDeclaration("1.0", "utf8", "yes"),
        //                    new XElement("Connections"));
        //doc = System.Xml.Linq.XDocument.Load(fileName);
        //doc.Save(fileName);
        //RootElementName = doc.Root.Name.LocalName;
        //WriteConfiguration(fileName, new ConfigurationDescriptor(new ObservableCollection<string>(new List<string> 
        //                { "http://example.localhost/fcgi-bin/finance", "http://www.example.somewebsite.com/fcgi-bin/finance" })));



        //using (XmlWriter writer = XmlWriter.Create(fileName))
        //{
        //    configurationDescriptor = (ConfigurationDescriptor)serializer.Serialize(writer);
        //}



        //FileStream fstream = GetFileStream(fileName);
        //return ReadConfiguratuion(fstream);

    }
}
