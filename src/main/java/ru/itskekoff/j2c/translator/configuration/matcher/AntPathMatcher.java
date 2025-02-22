package ru.itskekoff.j2c.translator.configuration.matcher;

import lombok.Getter;
import lombok.Setter;
import ru.itskekoff.j2c.translator.utils.StringUtils;

import java.util.*;
import java.util.concurrent.ConcurrentHashMap;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class AntPathMatcher {
    private static final char[] WILDCARD_CHARS;

    static {
        WILDCARD_CHARS = new char[]{'*', '?', '{'};
    }

    final Map<String, AntPathStringMatcher> stringMatcherCache;
    private final Map<String, String[]> tokenizedPatternCache;
    private final String pathSeparator;
    private final PathSeparatorPatternCache pathSeparatorPatternCache;
    @Setter
    private boolean caseSensitive;
    @Setter
    private boolean trimTokens;
    private volatile Boolean cachePatterns;

    public AntPathMatcher() {
        this.caseSensitive = true;
        this.trimTokens = false;
        this.tokenizedPatternCache = new ConcurrentHashMap<>(256);
        this.stringMatcherCache = new ConcurrentHashMap<>(256);
        this.pathSeparator = "/";
        this.pathSeparatorPatternCache = new PathSeparatorPatternCache("/");
    }

    private void deactivatePatternCache() {
        this.cachePatterns = false;
        this.tokenizedPatternCache.clear();
        this.stringMatcherCache.clear();
    }

    public boolean match(final String pattern, final String path) {
        return this.doMatch(pattern, path, true);
    }

    public boolean matchStart(final String pattern, final String path) {
        return this.doMatch(pattern, path, false);
    }

    protected boolean doMatch(final String pattern, final String path, final boolean fullMatch) {
        if (path == null || path.startsWith(this.pathSeparator) != pattern.startsWith(this.pathSeparator)) {
            return false;
        }
        final String[] pattDirs = this.tokenizePattern(pattern);
        if (fullMatch && this.caseSensitive && !this.isPotentialMatch(path, pattDirs)) {
            return false;
        }
        final String[] pathDirs = this.tokenizePath(path);
        int pattIdxStart;
        int pattIdxEnd;
        int pathIdxStart;
        int pathIdxEnd;
        for (pattIdxStart = 0, pattIdxEnd = pattDirs.length - 1, pathIdxStart = 0, pathIdxEnd = pathDirs.length - 1; pattIdxStart <= pattIdxEnd && pathIdxStart <= pathIdxEnd; ++pattIdxStart, ++pathIdxStart) {
            final String pattDir = pattDirs[pattIdxStart];
            if ("**".equals(pattDir)) {
                break;
            }
            if (this.matchStrings(pattDir, pathDirs[pathIdxStart])) {
                return false;
            }
        }
        if (pathIdxStart > pathIdxEnd) {
            if (pattIdxStart > pattIdxEnd) {
                return pattern.endsWith(this.pathSeparator) == path.endsWith(this.pathSeparator);
            }
            if (!fullMatch) {
                return true;
            }
            if (pattIdxStart == pattIdxEnd && pattDirs[pattIdxStart].equals("*") && path.endsWith(this.pathSeparator)) {
                return true;
            }
            for (int i = pattIdxStart; i <= pattIdxEnd; ++i) {
                if (!pattDirs[i].equals("**")) {
                    return false;
                }
            }
        } else {
            if (pattIdxStart > pattIdxEnd) {
                return false;
            }
            if (!fullMatch && "**".equals(pattDirs[pattIdxStart])) {
                return true;
            }
            while (pattIdxStart <= pattIdxEnd && pathIdxStart <= pathIdxEnd) {
                final String pattDir = pattDirs[pattIdxEnd];
                if (pattDir.equals("**")) {
                    break;
                }
                if (this.matchStrings(pattDir, pathDirs[pathIdxEnd])) {
                    return false;
                }
                --pattIdxEnd;
                --pathIdxEnd;
            }
            if (pathIdxStart > pathIdxEnd) {
                for (int i = pattIdxStart; i <= pattIdxEnd; ++i) {
                    if (!pattDirs[i].equals("**")) {
                        return false;
                    }
                }
                return true;
            }
            while (pattIdxStart != pattIdxEnd && pathIdxStart <= pathIdxEnd) {
                int patIdxTmp = -1;
                for (int j = pattIdxStart + 1; j <= pattIdxEnd; ++j) {
                    if (pattDirs[j].equals("**")) {
                        patIdxTmp = j;
                        break;
                    }
                }
                if (patIdxTmp == pattIdxStart + 1) {
                    ++pattIdxStart;
                } else {
                    final int patLength = patIdxTmp - pattIdxStart - 1;
                    final int strLength = pathIdxEnd - pathIdxStart + 1;
                    int foundIdx = -1;
                    int k = 0;
                    Label_0480:
                    while (k <= strLength - patLength) {
                        for (int l = 0; l < patLength; ++l) {
                            final String subPat = pattDirs[pattIdxStart + l + 1];
                            final String subStr = pathDirs[pathIdxStart + k + l];
                            if (this.matchStrings(subPat, subStr)) {
                                ++k;
                                continue Label_0480;
                            }
                        }
                        foundIdx = pathIdxStart + k;
                        break;
                    }
                    if (foundIdx == -1) {
                        return false;
                    }
                    pattIdxStart = patIdxTmp;
                    pathIdxStart = foundIdx + patLength;
                }
            }
            for (int i = pattIdxStart; i <= pattIdxEnd; ++i) {
                if (!pattDirs[i].equals("**")) {
                    return false;
                }
            }
        }
        return true;
    }

    private boolean isPotentialMatch(final String path, final String[] pattDirs) {
        if (!this.trimTokens) {
            int pos = 0;
            for (final String pattDir : pattDirs) {
                int skipped = this.skipSeparator(path, pos, this.pathSeparator);
                pos += skipped;
                skipped = this.skipSegment(path, pos, pattDir);
                if (skipped < pattDir.length()) {
                    return skipped > 0 || (!pattDir.isEmpty() && this.isWildcardChar(pattDir.charAt(0)));
                }
                pos += skipped;
            }
        }
        return true;
    }

    private int skipSegment(final String path, final int pos, final String prefix) {
        int skipped = 0;
        for (int i = 0; i < prefix.length(); ++i) {
            final char c = prefix.charAt(i);
            if (this.isWildcardChar(c)) {
                return skipped;
            }
            final int currPos = pos + skipped;
            if (currPos >= path.length()) {
                return 0;
            }
            if (c == path.charAt(currPos)) {
                ++skipped;
            }
        }
        return skipped;
    }

    private int skipSeparator(final String path, final int pos, final String separator) {
        int skipped;
        skipped = 0;
        while (path.startsWith(separator, pos + skipped)) {
            skipped += separator.length();
        }
        return skipped;
    }

    private boolean isWildcardChar(final char c) {
        for (final char candidate : AntPathMatcher.WILDCARD_CHARS) {
            if (c == candidate) {
                return true;
            }
        }
        return false;
    }

    protected String[] tokenizePattern(final String pattern) {
        String[] tokenized = null;
        final Boolean cachePatterns = this.cachePatterns;
        if (cachePatterns == null || cachePatterns) {
            tokenized = this.tokenizedPatternCache.get(pattern);
        }
        if (tokenized == null) {
            tokenized = this.tokenizePath(pattern);
            if (cachePatterns == null && this.tokenizedPatternCache.size() >= 65536) {
                this.deactivatePatternCache();
                return tokenized;
            }
            if (cachePatterns == null || cachePatterns) {
                this.tokenizedPatternCache.put(pattern, tokenized);
            }
        }
        return tokenized;
    }

    protected String[] tokenizePath(final String path) {
        return StringUtils.tokenizeToStringArray(path, this.pathSeparator, this.trimTokens, true);
    }

    private boolean matchStrings(final String pattern, final String str) {
        return !this.getStringMatcher(pattern).matchStrings(str, null);
    }

    protected AntPathStringMatcher getStringMatcher(final String pattern) {
        AntPathStringMatcher matcher = null;
        final Boolean cachePatterns = this.cachePatterns;
        if (cachePatterns == null || cachePatterns) {
            matcher = this.stringMatcherCache.get(pattern);
        }
        if (matcher == null) {
            matcher = new AntPathStringMatcher(pattern, this.caseSensitive);
            if (cachePatterns == null && this.stringMatcherCache.size() >= 65536) {
                this.deactivatePatternCache();
                return matcher;
            }
            if (cachePatterns == null || cachePatterns) {
                this.stringMatcherCache.put(pattern, matcher);
            }
        }
        return matcher;
    }

    public String combine(final String pattern1, final String pattern2) {
        final boolean pattern1ContainsUriVar = pattern1.indexOf(123) != -1;
        if (!pattern1.equals(pattern2) && !pattern1ContainsUriVar && this.match(pattern1, pattern2)) {
            return pattern2;
        }
        if (pattern1.endsWith(this.pathSeparatorPatternCache.getEndsOnWildCard())) {
            return this.concat(pattern1.substring(0, pattern1.length() - 2), pattern2);
        }
        if (pattern1.endsWith(this.pathSeparatorPatternCache.getEndsOnDoubleWildCard())) {
            return this.concat(pattern1, pattern2);
        }
        final int starDotPos1 = pattern1.indexOf("*.");
        if (pattern1ContainsUriVar || starDotPos1 == -1 || this.pathSeparator.equals(".")) {
            return this.concat(pattern1, pattern2);
        }
        final String ext1 = pattern1.substring(starDotPos1 + 1);
        final int dotPos2 = pattern2.indexOf(46);
        final String file2 = (dotPos2 == -1) ? pattern2 : pattern2.substring(0, dotPos2);
        final String ext2 = (dotPos2 == -1) ? "" : pattern2.substring(dotPos2);
        final boolean ext1All = ext1.equals(".*") || ext1.isEmpty();
        final boolean ext2All = ext2.equals(".*") || ext2.isEmpty();
        if (!ext1All && !ext2All) {
            throw new IllegalArgumentException("Cannot combine patterns: " + pattern1 + " vs " + pattern2);
        }
        final String ext3 = ext1All ? ext2 : ext1;
        return file2 + ext3;
    }

    private String concat(final String path1, final String path2) {
        final boolean path1EndsWithSeparator = path1.endsWith(this.pathSeparator);
        final boolean path2StartsWithSeparator = path2.startsWith(this.pathSeparator);
        if (path1EndsWithSeparator && path2StartsWithSeparator) {
            return path1 + path2.substring(1);
        }
        if (path1EndsWithSeparator || path2StartsWithSeparator) {
            return path1 + path2;
        }
        return path1 + this.pathSeparator + path2;
    }

    protected static class AntPathStringMatcher {
        private static final Pattern GLOB_PATTERN;

        static {
            GLOB_PATTERN = Pattern.compile("\\?|\\*|\\{((?:\\{[^/]+?}|[^/{}]|\\\\[{}])+?)}");
        }

        private final String rawPattern;
        private final boolean caseSensitive;
        private final boolean exactMatch;
        private final Pattern pattern;
        private final List<String> variableNames;


        public AntPathStringMatcher(final String pattern, final boolean caseSensitive) {
            this.variableNames = new ArrayList<>();
            this.rawPattern = pattern;
            this.caseSensitive = caseSensitive;
            final StringBuilder patternBuilder = new StringBuilder();
            final Matcher matcher = AntPathStringMatcher.GLOB_PATTERN.matcher(pattern);
            int end = 0;
            while (matcher.find()) {
                patternBuilder.append(this.quote(pattern, end, matcher.start()));
                final String match = matcher.group();
                if ("?".equals(match)) {
                    patternBuilder.append('.');
                } else if ("*".equals(match)) {
                    patternBuilder.append(".*");
                } else if (match.startsWith("{") && match.endsWith("}")) {
                    final int colonIdx = match.indexOf(58);
                    if (colonIdx == -1) {
                        patternBuilder.append("((?s).*)");
                        this.variableNames.add(matcher.group(1));
                    } else {
                        final String variablePattern = match.substring(colonIdx + 1, match.length() - 1);
                        patternBuilder.append('(');
                        patternBuilder.append(variablePattern);
                        patternBuilder.append(')');
                        final String variableName = match.substring(1, colonIdx);
                        this.variableNames.add(variableName);
                    }
                }
                end = matcher.end();
            }
            if (end == 0) {
                this.exactMatch = true;
                this.pattern = null;
            } else {
                this.exactMatch = false;
                patternBuilder.append(this.quote(pattern, end, pattern.length()));
                this.pattern = (this.caseSensitive ? Pattern.compile(patternBuilder.toString()) : Pattern.compile(patternBuilder.toString(), Pattern.CASE_INSENSITIVE));
            }
        }

        private String quote(final String s, final int start, final int end) {
            if (start == end) {
                return "";
            }
            return Pattern.quote(s.substring(start, end));
        }

        public boolean matchStrings(final String str, final Map<String, String> uriTemplateVariables) {
            if (this.exactMatch) {
                return this.caseSensitive ? this.rawPattern.equals(str) : this.rawPattern.equalsIgnoreCase(str);
            }
            if (this.pattern != null) {
                final Matcher matcher = this.pattern.matcher(str);
                if (matcher.matches()) {
                    if (uriTemplateVariables != null) {
                        if (this.variableNames.size() != matcher.groupCount()) {
                            throw new IllegalArgumentException("The number of capturing groups in the pattern segment " + this.pattern + " does not match the number of URI template variables it defines, which can occur if capturing groups are used in a URI template regex. Use non-capturing groups instead.");
                        }
                        for (int i = 1; i <= matcher.groupCount(); ++i) {
                            final String name = this.variableNames.get(i - 1);
                            if (name.startsWith("*")) {
                                throw new IllegalArgumentException("Capturing patterns (" + name + ") are not supported by the AntPathMatcher. Use the PathPatternParser instead.");
                            }
                            final String value = matcher.group(i);
                            uriTemplateVariables.put(name, value);
                        }
                    }
                    return true;
                }
            }
            return false;
        }
    }

    @Getter
    private static class PathSeparatorPatternCache {
        private final String endsOnWildCard;
        private final String endsOnDoubleWildCard;

        public PathSeparatorPatternCache(final String pathSeparator) {
            this.endsOnWildCard = pathSeparator + "*";
            this.endsOnDoubleWildCard = pathSeparator + "**";
        }

    }
}
