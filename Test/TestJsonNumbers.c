#include "TestJsonNumbers.h"


#include <assert.h>
#include <math.h>
#include <stdio.h>

#include "SEA/JSONParser.h"
#include "SEA/JSONValue.h"

static void test_number_parsing() {
    printf("Testing JSON Number Parsing...\n\n");

    // Test cases with different number formats
    const char *test_cases[] = {
        // Basic integers
        "42",
        "-42",
        "0",
        "-0",
        "123456789",
        "-987654321",

        // Decimal numbers
        "3.14159",
        "-2.71828",
        "0.5",
        "-0.25",
        "123.456",
        "-789.012",
        "0.0",
        "-0.0",
        "1.0",
        "-1.0",

        // Scientific notation (lowercase e)
        "1e5",
        "1e+5",
        "1e-5",
        "-1e5",
        "-1e+5",
        "-1e-5",
        "2.5e3",
        "2.5e+3",
        "2.5e-3",
        "-2.5e3",
        "-2.5e+3",
        "-2.5e-3",

        // Scientific notation (uppercase E)
        "1E5",
        "1E+5",
        "1E-5",
        "-1E5",
        "-1E+5",
        "-1E-5",
        "3.14E2",
        "3.14E+2",
        "3.14E-2",
        "-3.14E2",
        "-3.14E+2",
        "-3.14E-2",

        // Edge cases
        "0.000001",
        "-0.000001",
        "1000000.0",
        "-1000000.0",
        "1.23456789e10",
        "-9.87654321e-10",
        "0.0e0",
        "-0.0e0",
        "1.0e0",
        "-1.0e0",

        // Large numbers
        "1.7976931348623157e+308", // Close to double max
        "-1.7976931348623157e+308",
        "2.2250738585072014e-308", // Close to double min positive
        "-2.2250738585072014e-308",

        NULL // End marker
    };

    // Expected results (for verification)
    const double expected[] = {
        42.0, -42.0, 0.0, -0.0, 123456789.0, -987654321.0,
        3.14159, -2.71828, 0.5, -0.25, 123.456, -789.012, 0.0, -0.0, 1.0, -1.0,
        1e5, 1e+5, 1e-5, -1e5, -1e+5, -1e-5, 2.5e3, 2.5e+3, 2.5e-3, -2.5e3, -2.5e+3, -2.5e-3,
        1E5, 1E+5, 1E-5, -1E5, -1E+5, -1E-5, 3.14E2, 3.14E+2, 3.14E-2, -3.14E2, -3.14E+2, -3.14E-2,
        0.000001, -0.000001, 1000000.0, -1000000.0, 1.23456789e10, -9.87654321e-10, 0.0e0, -0.0e0, 1.0e0, -1.0e0,
        1.7976931348623157e+308, -1.7976931348623157e+308, 2.2250738585072014e-308, -2.2250738585072014e-308
    };

    int test_count = 0;
    int passed = 0;
    int failed = 0;

    for (int i = 0; test_cases[i] != NULL; i++) {
        const char *test_input = test_cases[i];

        // Parse the number
        struct SEA_JSONValue *result = SEA_JSONParser.FromString(test_input, 0, SEA_Allocator.Malloc);

        test_count++;

        if (result == NULL) {
            printf("❌ Test %d FAILED: '%s' -> NULL (parsing failed)\n", i + 1, test_input);
            failed++;
            continue;
        }

        if (result->type != SEA_JSON_NUMBER) {
            printf("❌ Test %d FAILED: '%s' -> wrong type (expected NUMBER, got %d)\n", i + 1, test_input, result->type);
            SEA_JSONValue.free(result, SEA_Allocator.Malloc);
            failed++;
            continue;
        }

        const double parsed_value = result->number;
        const double expected_value = expected[i];

        // Compare with small tolerance for floating point precision
        double diff = parsed_value - expected_value;
        if (diff < 0) diff = -diff;
        const double tolerance = expected_value == 0.0 ? 1e-15 : expected_value * 1e-12;
        if (diff <= fabs(tolerance)) {
            printf("✅ Test %d PASSED: '%s' -> %.17g\n", i + 1, test_input, parsed_value);
            passed++;
        } else {
            printf("❌ Test %d FAILED: '%s' -> %.17g (expected %.17g, diff=%.17g)\n",
                   i + 1, test_input, parsed_value, expected_value, diff);
            failed++;
        }

        SEA_JSONValue.free(result, SEA_Allocator.Malloc);
    }

    printf("\n=== Test Summary ===\n");
    printf("Total tests: %d\n", test_count);
    printf("Passed: %d\n", passed);
    printf("Failed: %d\n", failed);
    printf("Success rate: %.1f%%\n", (double) passed / test_count * 100.0);
}

static void test_complex_json_with_numbers() {
    printf("\n\nTesting Complex JSON with Various Numbers...\n\n");

    // Complex JSON with various number types
    const char *complex_json = "{\n"
            "  \"integers\": [0, 42, -17, 999999],\n"
            "  \"decimals\": [3.14159, -2.71828, 0.5, -0.25],\n"
            "  \"scientific\": [1e5, -2.5e-3, 3.14E+2, -9.87E-10],\n"
            "  \"edge_cases\": {\n"
            "    \"zero\": 0,\n"
            "    \"negative_zero\": -0,\n"
            "    \"small\": 0.000001,\n"
            "    \"large\": 1000000.0,\n"
            "    \"very_large\": 1.23456789e15,\n"
            "    \"very_small\": -4.56789e-12\n"
            "  },\n"
            "  \"mixed_array\": [\n"
            "    42,\n"
            "    \"string\",\n"
            "    -3.14,\n"
            "    true,\n"
            "    1.5e-5,\n"
            "    null,\n"
            "    -999,\n"
            "    false\n"
            "  ]\n"
            "}";

    struct SEA_JSONValue *json = SEA_JSONParser.FromString(complex_json, 0, SEA_Allocator.Malloc);

    if (json == NULL) {
        printf("❌ Failed to parse complex JSON\n");
        return;
    }

    if (json->type != SEA_JSON_OBJECT) {
        printf("❌ Expected JSON object, got type %d\n", json->type);
        SEA_JSONValue.free(json, SEA_Allocator.Malloc);
        return;
    }

    char *json_string = SEA_JSONValue.toString(json, SEA_Allocator.Malloc);
    printf("✅ Successfully parsed complex JSON:\n%s\n", json_string);

    SEA_Allocator.free(SEA_Allocator.Malloc, json_string);
    SEA_JSONValue.free(json, SEA_Allocator.Malloc);
}

static void test_invalid_numbers() {
    printf("\n\nTesting Invalid Number Cases...\n\n");

    const char *invalid_cases[] = {
        "01", // Leading zero not allowed
        "-01", // Leading zero not allowed
        "+42", // Leading + not allowed
        "42.", // Trailing decimal point
        ".42", // Missing integer part
        "-.42", // Missing integer part with negative
        "1.2.3", // Multiple decimal points
        "1e", // Incomplete exponent
        "1e+", // Incomplete exponent
        "1e-", // Incomplete exponent
        "1ee5", // Double exponent
        "1.5e2.3", // Decimal in exponent
        "", // Empty string
        "-", // Just minus sign
        ".", // Just decimal point
        "e5", // Just exponent
        "1.5e+2.3", // Decimal in positive exponent
        NULL
    };

    int invalid_test_count = 0;
    int correctly_rejected = 0;

    for (int i = 0; invalid_cases[i] != NULL; i++) {
        const char *test_input = invalid_cases[i];

        struct SEA_JSONValue *result = SEA_JSONParser.FromString(test_input, 0, SEA_Allocator.Malloc);

        invalid_test_count++;

        if (result == NULL) {
            printf("✅ Correctly rejected: '%s'\n", test_input);
            correctly_rejected++;
        } else {
            printf("❌ Should have rejected: '%s' but got %.17g\n", test_input,
                   result->type == SEA_JSON_NUMBER ? result->number : 0.0);
            SEA_JSONValue.free(result, SEA_Allocator.Malloc);
        }
    }

    printf("\n=== Invalid Cases Summary ===\n");
    printf("Total invalid tests: %d\n", invalid_test_count);
    printf("Correctly rejected: %d\n", correctly_rejected);
    printf("Incorrectly accepted: %d\n", invalid_test_count - correctly_rejected);
}

void testJsonNumbersParsing() {
    test_number_parsing();
    test_complex_json_with_numbers();
    test_invalid_numbers();
}
