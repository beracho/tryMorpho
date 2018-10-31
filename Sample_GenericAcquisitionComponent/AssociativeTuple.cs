using System.Collections.Generic;

namespace Sample_GenericAcquisitionComponent
{
	internal class AssociativeTuple<T1, T2>
	{
		private Dictionary<T1, T2> m_AssociativeTupleDictionary = new Dictionary<T1, T2>();

		public AssociativeTuple()
		{
		}

		public T2 this[T1 i_x_Type1]
		{
			get
			{
				if(!this.m_AssociativeTupleDictionary.ContainsKey(i_x_Type1))
					throw new KeyNotFoundException("The given key is not part of this associative tuple: " + i_x_Type1.ToString());

				return this.m_AssociativeTupleDictionary[i_x_Type1];
			}

			set
			{
				this.m_AssociativeTupleDictionary[i_x_Type1] = value;
			}
		}

		public T1 this[T2 i_x_Type2]
		{
			get
			{
				if(!this.m_AssociativeTupleDictionary.ContainsValue(i_x_Type2))
					throw new KeyNotFoundException("The given key is not part of this associative tuple: " + i_x_Type2.ToString());

				T1 l_x_Type1 = default(T1);

				foreach(KeyValuePair<T1, T2> l_x_KeyValuePair in this.m_AssociativeTupleDictionary)
				{
					if(l_x_KeyValuePair.Value.Equals(i_x_Type2))
					{
						l_x_Type1 = l_x_KeyValuePair.Key;
						break;
					}
				}

				return l_x_Type1;
			}

			set
			{
				try
				{
					T1 l_x_Type1 = this[i_x_Type2];

					if(!(l_x_Type1.Equals(value)))
					{
						this.m_AssociativeTupleDictionary.Remove(l_x_Type1);
						this.m_AssociativeTupleDictionary[value] = i_x_Type2;
					}
				}
				catch(KeyNotFoundException)
				{
					this.m_AssociativeTupleDictionary[value] = i_x_Type2;
				}
			}
		}

		public bool Contains(T1 i_x_Type1)
		{
			return this.m_AssociativeTupleDictionary.ContainsKey(i_x_Type1);
		}

		public bool Contains(T2 i_x_Type2)
		{
			return this.m_AssociativeTupleDictionary.ContainsValue(i_x_Type2);
		}

		public T1[] Firsts
		{
			get
			{
				T1[] l_x_Type1Array = new T1[this.m_AssociativeTupleDictionary.Keys.Count];

				this.m_AssociativeTupleDictionary.Keys.CopyTo(l_x_Type1Array, 0);

				return l_x_Type1Array;
			}
		}

		public T2[] Seconds
		{
			get
			{
				T2[] l_x_Type2Array = new T2[this.m_AssociativeTupleDictionary.Values.Count];

				this.m_AssociativeTupleDictionary.Values.CopyTo(l_x_Type2Array, 0);

				return l_x_Type2Array;
			}
		}
	}
}
