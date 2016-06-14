
import java.io.FileWriter;
import java.io.IOException;
import java.time.LocalDate;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;
import java.util.Map;

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
/**
 *
 * @author BBEIRIGO
 */
public class Util {

    /*TODO
     */
    /**
     * Save in file the data collected and sent by arduino via POST or GET
     *
     * @param filename Name of file
     * @param parameters The parameters passed via POST or GET
     */

    public static void saveInFile(String filename, Map<String, String[]> parameters) {
        //Open file in apped mode
        try (FileWriter a = new FileWriter(filename, true)) {
            //Make time stamp of data collected
            a.write("(" + LocalDate.now().format(DateTimeFormatter.ofPattern("YYYY-MM-dd")));
            a.write(" " + LocalTime.now().format(DateTimeFormatter.ofPattern("hh:mm:ss")) + ")");
            //Get all map entries passed via GET or POST
            for (Map.Entry<String, String[]> entry : parameters.entrySet()) {
                a.write(" - " + entry.getKey() + " {");
                for (int i = 0; i < entry.getValue().length; i++) {
                    a.write(entry.getValue()[i]);
                    if (i != entry.getValue().length - 1) {
                        a.write(", ");
                    }
                }
                a.write("}");
            }
            a.write("\n");
        } catch (IOException a) {
            a.printStackTrace();
        }
    }
}
