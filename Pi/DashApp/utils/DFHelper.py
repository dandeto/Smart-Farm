import pandas as pd

class DFHelper:
    def get_rows_where_col_val_num(df, field, value):
        return df[df[field] == value]

    def get_rows_where_col_val_num_comparator(df, field, value, operator):
        pass

    def get_rows_where_col_comparator(df, f1, operator, f2):
        pass

    def get_rows_where_col_val_is_str(df, field, value):
        return df[df[field.str.match(str(value))]

    def get_rows_where_col_val_is_str_exact(df, field, value):
        item = r'^' + str(value) + "$"
        return df[df[field].str.match(item)]

    def get_rows_where_col_val_is_list(df, field, value):
        return df[df[field].isin(value)]

    def get_col_where_val_is_multi_str_list(df, field, values):
        return df[df[field].isin(values)][field]

    def rm_row_where_col_values(df, field, values):
        selectedFields = df[df[field].isin(values)].index
        notSelectedFields = df[field].index
        to_remove = notSelectedFields.difference(selectedFields)
        return df.drop(to_remove)

    def rm_rows_where_col_is_val_exact(df, field, value):
        matched_vals_idx = df[df[field == value].index
        df = df.drop(matched_vals_idx)
        return df

    def rm_rows_where_col_val_is_str_exact(df, field, value):
        item = r'^' + str(value) + "$"
        selectedField = df[df[field].str.match(item)].index
        notSelectedField = df[field].index
        to_remove = notSelectedField.difference(selectedField)
        return df.drop(to_remove)       

    def convert_single_date_int_to_str(date):
        return dt.datetime.strptime(str(data), "%Y%m%d").strftime("%Y-%m-%d")

    def get_rows_from_date_range_index(df, field, sDate, eDate):
        return df.query(f'{sDate} <= {str(field)} <= {eDate}').index

    def get_rows_in_date_range(df, field, sDate, eDate):
        toKeepIdx = DFHelper.get_dates_from_range_index(df, field, sDate, eDate)
        allDatesIdx = df[field].index
        to_remove = allDatesIdx.difference(toKeepIdx)
        return df.drop(to_remove)

    def df_get_specific_cols(df, columns):
        keep = set(unique_l(columns))
        remove = list(set(unique_l(df.columns.values.tolist())) ^ keep)
        return df.drop(columns=remove, errors='raise')

    def replace_vals_in_col(df, column_name, old_new_map):
        """dataframe, string column name in df, 
        {"old_value": "new_value", "old_value": "new_value"} """
        return df.replace({str(column_name): old_new_map})
