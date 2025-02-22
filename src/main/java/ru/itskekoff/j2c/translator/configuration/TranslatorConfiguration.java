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
            @CommentValue(value = "* - совпадение по следующим символам (ShitClass -> Shit*)", commentValueIndent = 1)
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

        @Comment(@CommentValue(value = "Включает подробную информацию о компиляции (дебаг)", commentValueIndent = 1))
        public boolean DEBUG_ENABLED = false;
    }

    /*
    public Compiler COMPILER = new Compiler();
    @Comment(@CommentValue(value = "Настройки сборки .dll", commentValueIndent = 1))
    public static class Compiler {
        @Comment({
                @CommentValue(value = "Таргеты сборки")
        })
        public List<String> TARGETS = List.of("windows_x86_64", "windows_aarch64");
    }
     */
}
