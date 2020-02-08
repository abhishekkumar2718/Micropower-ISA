#ifndef LABEL_H
#define LABEL_H

typedef std::string Symbol;
typedef std::string Datatype;

class Label
{
  private:
    Datatype data_type;
    std::string expr;

    bool valid_data_type() const;
  public:
    // Symbol (i.e name of the label)
    Symbol symbol;
    Label(const std::string);
    friend std::ostream& operator<<(std::ostream&, const Label&);

    // Size of each element
    unsigned int element_size() const;

    // Number of elements
    unsigned int n_elements() const;

    // Size occupied by the label
    unsigned int size() const;
};

#endif
