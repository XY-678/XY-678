from sklearn.model_selection import train_test_split
from sklearn.naive_bayes import MultinomialNB#先验概率为多项式分布
from sklearn.svm import SVC
import data_process


#数据集划分为训练集和验证集
X_train, X_val, Y_train, Y_val = train_test_split(X_train, Y_train, test_size=0.2, random_state=42)