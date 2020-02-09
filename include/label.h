/*
 Labels in assembly are equivalent to variables are in other programming
 langauges. They have an identify (`symbol`), data type and possibly
 initialisation expression (`expr`).
 */
#ifndef LABEL_H
#define LABEL_H

class Label
{
  private:
    std::string data_type;
    std::string expr;

    bool valid_data_type() const;
  public:
    std::string symbol;

    Label(const std::string);

    // Size of each element
    unsigned int element_size() const;

    // Number of elements
    unsigned int n_elements() const;

    // Size occupied by the label
    unsigned int size() const;

    // Initialize expression at given memory address
    void fill(char*) const;

    friend std::ostream& operator<<(std::ostream&, const Label&);
};

#endif
