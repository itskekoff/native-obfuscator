package ru.itskekoff.j2c.annotations.vmp;

import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;

/**
 * @author itskekoff
 * @since 16:14 of 19.02.2025
 */
@Retention(RetentionPolicy.RUNTIME)
public @interface VMProtect {
    VMProtectType type();
}
