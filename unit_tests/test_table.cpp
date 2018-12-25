/**
 * @file test_table.cpp
 * @brief Unit tests for 2d/3d lookup tables
 * 
 * @date Dec 24, 2018
 * @author Matthew Kennedy, (c) 2018
 */

#include "test_table.h"

#include "Table.h"

#include "unit_test_framework.h"

void assert_binresult(BinResult expected, BinResult actual)
{
    // First check index match
    assertEquals(expected.Idx, actual.Idx);

    // Don't check exact equal because that doesn't work with floats
    assertEquals(true, fabs(expected.Frac - actual.Frac) < 0.01f);
}

void test_bins_2()
{
    printf("Test bins 2\n");

    float b[] = { 0, 10 };
    TableBins<float, 2> bins(b);

    // off scale low
    assert_binresult({ 0, 0.0f }, bins.Get(-10));
    assert_binresult({ 0, 0.0f }, bins.Get(-1));

    // Exactly on left bound
    assert_binresult({ 0, 0.0f }, bins.Get(0));

    // mid scale
    assert_binresult({ 0, 0.1f }, bins.Get(1));
    assert_binresult({ 0, 0.5f }, bins.Get(5));
    assert_binresult({ 0, 0.9f }, bins.Get(9));

    // off scale high
    assert_binresult({ 0, 1.0f }, bins.Get(11));
    assert_binresult({ 0, 1.0f }, bins.Get(50));
}

void test_bins_3()
{
    printf("Test bins 3\n");

    float b[] = { 0, 10, 20 };
    TableBins<float, 3> bins(b);

    // off scale low
    assert_binresult({ 0, 0.0f }, bins.Get(-10));
    assert_binresult({ 0, 0.0f }, bins.Get(-1));

    // Exactly on left bound
    assert_binresult({ 0, 0.0f }, bins.Get(0));

    // in between points 0 and 1
    assert_binresult({ 0, 0.1f }, bins.Get(1));
    assert_binresult({ 0, 0.5f }, bins.Get(5));
    assert_binresult({ 0, 0.9f }, bins.Get(9));

    // point near mid point
    assert_binresult({ 0, 0.999f }, bins.Get(9.99));
    assert_binresult({ 1, 0.0f }, bins.Get(10));
    assert_binresult({ 1, 0.01f }, bins.Get(10.01));

    // in between points 1 and 2
    assert_binresult({ 1, 0.1f }, bins.Get(11));
    assert_binresult({ 1, 0.5f }, bins.Get(15));
    assert_binresult({ 1, 0.9f }, bins.Get(19));

    // off scale high
    assert_binresult({ 1, 1.0f }, bins.Get(21));
    assert_binresult({ 1, 1.0f }, bins.Get(50));
}

void test_bins()
{
    test_bins_2();
    test_bins_3();
}

void test_table2d_2()
{
    printf("Test table2d 2\n");

    float bins[] = { 10, 20 };
    float values[] = { 40, 30 };
    Table2d<float, float, 2> table("unit test table2d", bins, values);

    // Off scale low
    assertEquals(40, table.Get(0));
    assertEquals(40, table.Get(9.99));

    // Exactly on left bound
    assertEquals(40, table.Get(10));
    
    // mid scale
    assertEquals(39, table.Get(11));
    assertEquals(35, table.Get(15));
    assertEquals(31, table.Get(19));

    // Exactly on right bound
    assertEquals(30, table.Get(20));

    // Off scale high
    assertEquals(30, table.Get(20.1));
    assertEquals(30, table.Get(50));
}

void test_table2d_3()
{
    printf("Test table2d 3\n");

    float bins[] = { 10, 20, 30 };
    float values[] = { 110, 120, 130 };
    Table2d<float, float, 3> table("unit test table2d", bins, values);

    // Off scale low
    assertEquals(110, table.Get(0));
    assertEquals(110, table.Get(9.99));

    // Exactly on left bound
    assertEquals(110, table.Get(10));
    
    // mid scale bottom half
    assertEquals(115, table.Get(15));

    // Exactly on middle point
    assertEquals(120, table.Get(20));

    // mid scale top half
    assertEquals(125, table.Get(25));

    // Exactly on top point
    assertEquals(130, table.Get(30));

    // Off scale high
    assertEquals(130, table.Get(30.1));
    assertEquals(130, table.Get(50));
}

void test_table2d()
{
    test_table2d_2();
    test_table2d_3();
}

void test_table()
{
    test_bins();
    test_table2d();
    //test_table3d();
}
