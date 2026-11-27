from ve_autotune.pipeline.binning import nearest_cell

RPM_BINS = [800, 2000, 4000, 6000]
LOAD_BINS = [30, 60, 90, 120]


def test_nearest_cell_exact_match():
    assert nearest_cell(2000, 60, RPM_BINS, LOAD_BINS) == (1, 1)


def test_nearest_cell_rounds_to_closer_edge():
    # 2900 closer to 2000 than 4000; 50 closer to 60 than 30
    assert nearest_cell(2900, 50, RPM_BINS, LOAD_BINS) == (0, 1)


def test_nearest_cell_below_range_clamps_to_first():
    assert nearest_cell(100, 10, RPM_BINS, LOAD_BINS) == (0, 0)


def test_nearest_cell_above_range_clamps_to_last():
    assert nearest_cell(9000, 500, RPM_BINS, LOAD_BINS) == (3, 3)
