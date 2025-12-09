import java.io.*;

String color1 = "0x9838";  //Цвет в битапе который нужно заменить на цвет заднего фона     
String color2 = "0x039f";  //Цвет в битмапе который нужно заменить на цвет главного меню  
String color1Name = "FONE_COLOR"; 
String color2Name = "TEXT_COLOR"; 

void setup() {
  processBitmap();
  exit();
}

void processBitmap() {
  String inputFile = "bitmap.txt";
  
  try {
    // Считываем все строки
    String[] lines = loadStrings(inputFile);
    StringBuilder sb = new StringBuilder();

    for (String line : lines) {
      // Заменяем все вхождения
      line = line.replace(color1, color1Name);
      line = line.replace(color2, color2Name);
      sb.append(line).append("\n");
    }

    // Сохраняем новый файл
    saveStrings("bitmap.txt", split(sb.toString(), "\n"));
    println("Готово! Файл saved as: converted_bitmap.txt");

  } catch (Exception e) {
    println("Ошибка при обработке файла: " + e.getMessage());
  }
}
