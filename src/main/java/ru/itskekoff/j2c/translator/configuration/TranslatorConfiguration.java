package ru.itskekoff.j2c.translator.configuration;

import net.elytrium.serializer.annotations.Comment;
import net.elytrium.serializer.annotations.CommentValue;
import net.elytrium.serializer.language.object.YamlSerializable;

import java.util.List;

public class TranslatorConfiguration extends YamlSerializable {
    public static final TranslatorConfiguration IMP = new TranslatorConfiguration();

    public Main MAIN = new Main();
    @Comment({
            @CommentValue(value = "Основные настройки нативного транслятора .java кода", commentValueIndent = 1),
            @CommentValue(value = "Используется стиль ant -> dev/jnic/web/**; dev.jnic.web.Shit* (https://stackoverflow.com/a/22636142)", commentValueIndent = 1),
            @CommentValue(value = "** - совпадение по внутренним путям и именам", commentValueIndent = 1),
            @CommentValue(value = "* - совпадение по следующим символам (ShitClass -> Shit*)", commentValueIndent = 1),
            @CommentValue(value = "Пустые includes и excludes (нулевое значение пустое) дают понять, что включать надо ВСЕ классы", commentValueIndent = 1)
    })
    public static class Main {
        @Comment(@CommentValue(value = "Включает в компиляцию класс(-ы)", commentValueIndent = 1))
        public List<String> INCLUDES = List.of("dev/**");
        @Comment(@CommentValue(value = "Исключает из компиляции класс(-ы)", commentValueIndent = 1))
        public List<String> EXCLUDES = List.of("codes/**");

        @Comment({
                @CommentValue(value = "Включает использование аннотаций для включения/исключения", commentValueIndent = 1),
                @CommentValue(value = "Полностью отключает два предыдущих параметра", commentValueIndent = 1)
        })
        public boolean USE_ANNOTATIONS = false;

        @Comment(@CommentValue(value = "Очищать аннотации VMProtect", commentValueIndent = 1))
        public boolean CLEAN_VMPROTECT_ANNOTATIONS = true;

        @Comment(@CommentValue(value = "Включает подробную информацию о компиляции (дебаг)", commentValueIndent = 1))
        public boolean DEBUG_ENABLED = false;
    }

    public Reference REFERENCE = new Reference();
    @Comment(@CommentValue(value = "Настройки работы с референсами", commentValueIndent = 1))
    public static class Reference {
        @Comment(@CommentValue(value = "Ссылка на сервер, в котором референсы частично декриптятся", commentValueIndent = 1))
        public String REFERENCE_URL = "http://localhost:6555/decrypt";

        @Comment(@CommentValue(value = "Начальная константа сида", commentValueIndent = 1))
        public long SEED_CONSTANT = 583485834L;
        @Comment(@CommentValue(value = "Фактор рандома", commentValueIndent = 1))
        public long RANDOM_FACTOR = 6364136222846791005L;
        @Comment(@CommentValue(value = "Начальный ключ для ксора сида", commentValueIndent = 1))
        public int XOR_CONSTANT = 0xDC9A;
    }
}
