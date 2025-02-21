package ru.itskekoff.j2c.translator.utils.resource;

import ru.itskekoff.j2c.translator.TranslatorMain;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;

/**
 * @author itskekoff
 * @since 15:24 of 21.02.2025
 */
public class ResourceUtils {
    public static String getStringFromResource(String resourcePath) throws IOException {
        InputStream inputStream = ResourceUtils.class.getResourceAsStream(resourcePath);
        if (inputStream == null) {
            TranslatorMain.LOGGER.error("Resource not found (path={})", resourcePath);
            return "";
        }

        StringBuilder content = new StringBuilder();
        try (BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream))) {
            String line;
            while ((line = reader.readLine()) != null) {
                content.append(line).append(System.lineSeparator());
            }
        }
        return content.toString();
    }

}
